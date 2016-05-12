#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/device.h>

#include <asm/arch/regs-gpio.h>
#include <asm/arch/regs-timer.h>
#include <asm/hardware.h>
#include <asm/io.h>


#define DEVICE_NAME	"buzzer"

static int majorno = 250;//���豸��
static int minorno = 0;//���豸��

//��������
static int buzzer_open (struct inode *,struct file *);
static int buzzer_release (struct inode *,struct file *);
static int buzzer_ioctl (struct inode *,struct file *,unsigned int,unsigned long);
static int __init buzzer_init(void);
static void __exit buzzer_exit(void);

struct file_operations buzzer_fops = 
{
	.owner = THIS_MODULE,
	.open = buzzer_open,
	.release = buzzer_release,
	.ioctl = buzzer_ioctl,
};

struct buzzer_dev
{
	struct cdev cdev;
};

struct buzzer_dev *my_dev;

static int __init buzzer_init(void)
{	
	int ret = 0, err = 0;
	dev_t devno = MKDEV(majorno,minorno);
	ret = alloc_chrdev_region(&devno,minorno,1,DEVICE_NAME); //minorno:ָ���ĵ�һ�����豸�ţ����ȶ�̬����
	/*if (ret < 0)										 //�����̬���䲻�ɹ���̬����
	{	
		ret = register_chrdev_region(devno,1,DEVICE_NAME);
	}*/
	
	if (ret < 0)										 //��̬�;�̬���䶼���ɹ�
	{

		printk(KERN_ALERT"Buzzer register failure!\n");//������Ϣ������������ȡ��ʩ
		buzzer_exit();		
		return ret;
	}
	else
	{
		printk(KERN_NOTICE"Buzzer register success!\n");//�������������ѳɹ�
		majorno = MAJOR(devno);//������䵽�����豸��
		my_dev = kmalloc(sizeof(struct buzzer_dev),GFP_KERNEL);//���ڴ����ʱ����
		if (!my_dev)
		{
			printk(KERN_ALERT"Creat device failed!\n");
			ret = -ENOMEM;
		}
		else
		{
			printk(KERN_NOTICE"Creat device success!\n");
			cdev_init(&my_dev->cdev,&buzzer_fops);
			my_dev->cdev.owner = THIS_MODULE;
			my_dev->cdev.ops = &buzzer_fops;
			err = cdev_add(&my_dev->cdev,devno,1);
		/*	if (err)
			{
				printk(KERN_ALERT"Add device failure!\n");
			}*/
		}
	}
	
	return ret;
}

static void __exit buzzer_exit(void)
{
	dev_t devno = MKDEV(majorno,minorno);//���������豸��ת����dev_t����,dev_t�������������������豸��
	cdev_del(&my_dev->cdev);//�ͷ�cdevռ�õ��ڴ�
	kfree(my_dev);//
	unregister_chrdev_region(devno,1);
	printk(KERN_NOTICE"Buzzer unregister success!\n");
}

static int buzzer_open (struct inode *inode,struct file *filp)
{
	struct buzzer_dev *dev;
	dev = container_of(inode->i_cdev,struct buzzer_dev,cdev);
	filp->private_data = dev;
 	s3c2410_gpio_cfgpin(S3C2410_GPB0,S3C2410_GPB0_TOUT0);//����GPB0ΪTOUT0	
	return 0;
}

static int buzzer_release (struct inode *inode,struct file *filp)
{
	return 0;
}

static int buzzer_ioctl (struct inode *inode,struct file *filp,unsigned int cmd,unsigned long arg)
{
	if (cmd <= 0)
	{	
		s3c2410_gpio_cfgpin(S3C2410_GPB0,S3C2410_GPB0_OUTP);//�رշ�����
		s3c2410_gpio_setpin(S3C2410_GPB0,0);
	}
	
	else
	{	
		unsigned long tcnt = 0;
		unsigned long tcon = 0;
		unsigned long tcfg0 = 0;
		unsigned long tcfg1 = 0;
		unsigned long pclk = 0;
	

		tcfg0 = __raw_readl(S3C2410_TCFG0); //��ʱ�����üĴ���
		tcfg1 = __raw_readl(S3C2410_TCFG1);
		tcfg0 &= ~S3C2410_TCFG_PRESCALER0_MASK; //��Ӧλ����
		tcfg0 |= (50-1);//49 ��ʱ��Ԥ��Ƶֵ
		tcfg1 &= ~S3C2410_TCFG1_MUX0_MASK; //��Ӧλ���� 
		tcfg1 |= S3C2410_TCFG1_MUX0_DIV4;//1/4 ��Ƶֵ PWM��ʱ��0ѡͨѡ��DIV4
		__raw_writel(tcfg0,S3C2410_TCFG0);//д��Ĵ���
		__raw_writel(tcfg1,S3C2410_TCFG1);



		pclk = 50000000;   //ʱ��50MHZ
		tcnt = (pclk/50/4)/cmd;//�����ӦcmdƵ�ʵĶ�ʱ������ֵ   ��ʱ�����ƼĴ���  PCLK/Ԥ��Ƶֵ + 1(0~255)/��Ƶֵ
		__raw_writel(tcnt,S3C2410_TCNTB(0));//��ʱ��0��������Ĵ���
		__raw_writel(tcnt/2,S3C2410_TCMPB(0)); //��ʱ��0�Ƚϻ�����
		tcon = __raw_readl(S3C2410_TCON);
		tcon	&= ~0x1f;  //00011111  ��Ӧλ���� 
		tcon |= 0xb;//��ֹ����0���Զ�����1���رձ���0������1��������ʱ��1  01011 
		__raw_writel(tcon,S3C2410_TCON);
		tcon &= ~0x2;//����ֶ�����//0010
		__raw_writel(tcon,S3C2410_TCON);
	}
	
	return 0;
}

module_init(buzzer_init);
module_exit(buzzer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("huangting");
MODULE_DESCRIPTION("make by huangting");



