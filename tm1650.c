#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/rtc.h>
#include <linux/of.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/printk.h>

#define  DEVICE_NAME            "tm1650"
#define  TM1650_NAME			"tm1650"

#define TM1650_SEG_1			0x1f
#define TM1650_SEG_2			0x2f
#define TM1650_SEG_3			0x3f
#define TM1650_SEG_4			0x4f

#define TM1650_CHAR_0			0xf0
#define TM1650_CHAR_1			0xf1
#define TM1650_CHAR_2			0xf2
#define TM1650_CHAR_3			0xf3
#define TM1650_CHAR_4			0xf4
#define TM1650_CHAR_5			0xf5
#define TM1650_CHAR_6			0xf6
#define TM1650_CHAR_7			0xf7
#define TM1650_CHAR_8			0xf8
#define TM1650_CHAR_9			0xf9


// Helper macro to create combined macros
#define COMBINE(seg, chr)       ((seg) & (chr))

// Combined macros
#define TM1650_SEG_1_CHAR_0     COMBINE(TM1650_SEG_1, TM1650_CHAR_0)
#define TM1650_SEG_1_CHAR_1     COMBINE(TM1650_SEG_1, TM1650_CHAR_1)
#define TM1650_SEG_1_CHAR_2     COMBINE(TM1650_SEG_1, TM1650_CHAR_2)
#define TM1650_SEG_1_CHAR_3     COMBINE(TM1650_SEG_1, TM1650_CHAR_3)
#define TM1650_SEG_1_CHAR_4     COMBINE(TM1650_SEG_1, TM1650_CHAR_4)
#define TM1650_SEG_1_CHAR_5     COMBINE(TM1650_SEG_1, TM1650_CHAR_5)
#define TM1650_SEG_1_CHAR_6     COMBINE(TM1650_SEG_1, TM1650_CHAR_6)
#define TM1650_SEG_1_CHAR_7     COMBINE(TM1650_SEG_1, TM1650_CHAR_7)
#define TM1650_SEG_1_CHAR_8     COMBINE(TM1650_SEG_1, TM1650_CHAR_8)
#define TM1650_SEG_1_CHAR_9     COMBINE(TM1650_SEG_1, TM1650_CHAR_9)

#define TM1650_SEG_2_CHAR_0     COMBINE(TM1650_SEG_2, TM1650_CHAR_0)
#define TM1650_SEG_2_CHAR_1     COMBINE(TM1650_SEG_2, TM1650_CHAR_1)
#define TM1650_SEG_2_CHAR_2     COMBINE(TM1650_SEG_2, TM1650_CHAR_2)
#define TM1650_SEG_2_CHAR_3     COMBINE(TM1650_SEG_2, TM1650_CHAR_3)
#define TM1650_SEG_2_CHAR_4     COMBINE(TM1650_SEG_2, TM1650_CHAR_4)
#define TM1650_SEG_2_CHAR_5     COMBINE(TM1650_SEG_2, TM1650_CHAR_5)
#define TM1650_SEG_2_CHAR_6     COMBINE(TM1650_SEG_2, TM1650_CHAR_6)
#define TM1650_SEG_2_CHAR_7     COMBINE(TM1650_SEG_2, TM1650_CHAR_7)
#define TM1650_SEG_2_CHAR_8     COMBINE(TM1650_SEG_2, TM1650_CHAR_8)
#define TM1650_SEG_2_CHAR_9     COMBINE(TM1650_SEG_2, TM1650_CHAR_9)

#define TM1650_SEG_3_CHAR_0     COMBINE(TM1650_SEG_3, TM1650_CHAR_0)
#define TM1650_SEG_3_CHAR_1     COMBINE(TM1650_SEG_3, TM1650_CHAR_1)
#define TM1650_SEG_3_CHAR_2     COMBINE(TM1650_SEG_3, TM1650_CHAR_2)
#define TM1650_SEG_3_CHAR_3     COMBINE(TM1650_SEG_3, TM1650_CHAR_3)
#define TM1650_SEG_3_CHAR_4     COMBINE(TM1650_SEG_3, TM1650_CHAR_4)
#define TM1650_SEG_3_CHAR_5     COMBINE(TM1650_SEG_3, TM1650_CHAR_5)
#define TM1650_SEG_3_CHAR_6     COMBINE(TM1650_SEG_3, TM1650_CHAR_6)
#define TM1650_SEG_3_CHAR_7     COMBINE(TM1650_SEG_3, TM1650_CHAR_7)
#define TM1650_SEG_3_CHAR_8     COMBINE(TM1650_SEG_3, TM1650_CHAR_8)
#define TM1650_SEG_3_CHAR_9     COMBINE(TM1650_SEG_3, TM1650_CHAR_9)

#define TM1650_SEG_4_CHAR_0     COMBINE(TM1650_SEG_4, TM1650_CHAR_0)
#define TM1650_SEG_4_CHAR_1     COMBINE(TM1650_SEG_4, TM1650_CHAR_1)
#define TM1650_SEG_4_CHAR_2     COMBINE(TM1650_SEG_4, TM1650_CHAR_2)
#define TM1650_SEG_4_CHAR_3     COMBINE(TM1650_SEG_4, TM1650_CHAR_3)
#define TM1650_SEG_4_CHAR_4     COMBINE(TM1650_SEG_4, TM1650_CHAR_4)
#define TM1650_SEG_4_CHAR_5     COMBINE(TM1650_SEG_4, TM1650_CHAR_5)
#define TM1650_SEG_4_CHAR_6     COMBINE(TM1650_SEG_4, TM1650_CHAR_6)
#define TM1650_SEG_4_CHAR_7     COMBINE(TM1650_SEG_4, TM1650_CHAR_7)
#define TM1650_SEG_4_CHAR_8     COMBINE(TM1650_SEG_4, TM1650_CHAR_8)
#define TM1650_SEG_4_CHAR_9     COMBINE(TM1650_SEG_4, TM1650_CHAR_9)

struct tm1650_misc{
	struct i2c_client *client;
};

static struct tm1650_misc tm1650_info = {
	.client = NULL,
};

static const struct of_device_id imx258_of_match[] = {
	{ .compatible = "mok,i2c-led-panel" },
	{},
};

static const struct i2c_device_id tm1650_id[] = {
	{"mok,i2c-led-panel", 0},
	{},
};

static int tm1650_open(struct inode *inode, struct file *file)
{
	return 0;
};

static int tm1650_release(struct inode *inode, struct file *file)
{
	return 0;
}
// 48作为地址
// 先发00设置8级亮度， （i2c的寄存器地址写）
// 再发00（08表示7段显示）设8段显示		（数据）

// 显存地址
// 68
// 6a
// 6c
// 6e


//可写数据
//	0x3F,	// 0
//	0x06,	// 1
//	0x5B,	// 2
//	0x4F,	// 3
//	0x66,	// 4
//	0x6D,	// 5
//	0x7D,	// 6
//	0x07,	// 7
//	0x7F,	// 8
//	0x6F,	// 9

static long tm1650_compat_ioctl(struct file *file, unsigned int cmd,
			       unsigned long parm)
{
	pr_info("[%s,%d]goes here\n", __func__, __LINE__);
	//switch (cmd)
	{
		//break;
	}
	return 0;
}

int find_address(u8 pos, u8 *addr)
{
	static u8 address_list[5] = {0xff, 0x68, 0x6a, 0x6c, 0x6e};
	if (pos > 4 || pos < 1)
	{
		pr_info("[%s,%d]pos not supported, input pos: %hhu\n", __func__, __LINE__, pos);
		return -1;
	}
	
	if (addr == NULL)
	{
		pr_info("[%s,%d]param addr is null\n", __func__, __LINE__);
		return -1;
	}
	
	*addr = address_list[pos];
	return 0;
}

int find_char_code(u8 value, u8 *code)
{
	// value 0-f, code 0-15
	// 3F 06 5B 4F 66 6D 7D 07 7F 6F 77 7C 39 5E 79 71
	// value 0.-f. code 16-31
	// BF 86 DB CF E6 ED FD 87 FF DF F7 FC B9 DE F9 F1
	static u8 code_list[32] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xDF, 0xF7, 0xFC, 0xB9, 0xDE, 0xF9, 0xF1};
	if (value > 31)
	{
		pr_info("[%s,%d]value not supported, input value: %hhu\n", __func__, __LINE__, value);
		return -1;
	}
	
	if (code == NULL)
	{
		pr_info("[%s,%d]param code is null\n", __func__, __LINE__);
		return -1;
	}
	
	*code = code_list[value];
	return 0;
}

ssize_t set_first(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	// 0x68 0x6a 0x6c 0x6e
	struct i2c_msg msgs[1];
	u8 addr = 0x00;
	u8 value = 0;
	u8 value_code = 0x00;
	int ret = 0;
	
	if (buf == NULL)
	{
		pr_info("[%s,%d]wrong parameters\n", __func__, __LINE__);
		return -1;
	}
	
	if ((buf[0] - '0') < 0 || (buf[0] - '0') > 31)
	{
		pr_info("[%s,%d]wrong parameters, char value %hhu is not supported\n", __func__, __LINE__, (u8)(buf[0] - '0'));
		return -1;
	}
	
	value = buf[0] - '0';
	ret = find_address(1, &addr);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_address failed\n", __func__, __LINE__);
		return -1;
	}
	ret = find_char_code(value, &value_code);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_char_code failed, value:%hhu\n", __func__, __LINE__, value);
		return -1;
	}
	
	msgs[0].addr = addr >> 1;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &value_code;
	ret = i2c_transfer(tm1650_info.client->adapter, &(msgs[0]), 1);
	if (ret < 0)
	{
		pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
		return -1;
	}
	
	return count;
}

ssize_t set_second(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_msg msgs[1];
	u8 addr = 0x00;
	u8 value = 0;
	u8 value_code = 0x00;
	int ret = 0;
	
	if (buf == NULL)
	{
		pr_info("[%s,%d]wrong parameters\n", __func__, __LINE__);
		return -1;
	}
	
	if ((buf[0] - '0') < 0 || (buf[0] - '0') > 31)
	{
		pr_info("[%s,%d]wrong parameters, char value %hhu is not supported\n", __func__, __LINE__, (u8)(buf[0] - '0'));
		return -1;
	}
	
	value = buf[0] - '0';
	ret = find_address(2, &addr);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_address failed\n", __func__, __LINE__);
		return -1;
	}
	ret = find_char_code(value, &value_code);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_char_code failed, value:%hhu\n", __func__, __LINE__, value);
		return -1;
	}
	
	msgs[0].addr = addr >> 1;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &value_code;
	ret = i2c_transfer(tm1650_info.client->adapter, &(msgs[0]), 1);
	if (ret < 0)
	{
		pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
		return -1;
	}
	
	return count;
}

ssize_t set_third(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_msg msgs[1];
	u8 addr = 0x00;
	u8 value = 0;
	u8 value_code = 0x00;
	int ret = 0;
	
	if (buf == NULL)
	{
		pr_info("[%s,%d]wrong parameters\n", __func__, __LINE__);
		return -1;
	}
	
	if ((buf[0] - '0') < 0 || (buf[0] - '0') > 31)
	{
		pr_info("[%s,%d]wrong parameters, char value %hhu is not supported\n", __func__, __LINE__, (u8)(buf[0] - '0'));
		return -1;
	}
	
	value = buf[0] - '0';
	ret = find_address(3, &addr);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_address failed\n", __func__, __LINE__);
		return -1;
	}
	ret = find_char_code(value, &value_code);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_char_code failed, value:%hhu\n", __func__, __LINE__, value);
		return -1;
	}
	
	msgs[0].addr = addr >> 1;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &value_code;
	ret = i2c_transfer(tm1650_info.client->adapter, &(msgs[0]), 1);
	if (ret < 0)
	{
		pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
		return -1;
	}
	
	return count;
}

ssize_t set_fourth(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_msg msgs[1];
	u8 addr = 0x00;
	u8 value = 0;
	u8 value_code = 0x00;
	int ret = 0;
	
	if (buf == NULL)
	{
		pr_info("[%s,%d]wrong parameters\n", __func__, __LINE__);
		return -1;
	}
	
	if ((buf[0] - '0') < 0 || (buf[0] - '0') > 31)
	{
		pr_info("[%s,%d]wrong parameters, char value %hhu is not supported\n", __func__, __LINE__, (u8)(buf[0] - '0'));
		return -1;
	}
	
	value = buf[0] - '0';
	ret = find_address(4, &addr);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_address failed\n", __func__, __LINE__);
		return -1;
	}
	ret = find_char_code(value, &value_code);
	if (ret < 0)
	{
		pr_info("[%s,%d]find_char_code failed, value:%hhu\n", __func__, __LINE__, value);
		return -1;
	}
	
	msgs[0].addr = addr >> 1;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &value_code;
	ret = i2c_transfer(tm1650_info.client->adapter, &(msgs[0]), 1);
	if (ret < 0)
	{
		pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
		return -1;
	}
	
	return count;
}

ssize_t set_brightness(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_msg msgs[1];
	int ret = 0;
	u8 brightness_to_set = 0;
	u8 buf1 = 0;
	
	//8级别亮度，级别1到8代码：0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x01
	static u8 brightness[9] = {0x01, 0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x01};
	
	if (buf == NULL)
	{
		pr_info("[%s,%d]wrong parameters\n", __func__, __LINE__);
		return -1;
	}
	
	if ((buf[0] - '0') < 1 || (buf[0] - '0') > 8)
	{
		pr_info("[%s,%d]wrong parameters, brightness level %hhu is not supported\n", __func__, __LINE__, (u8)(buf[0] - '0'));
		return -1;
	}
	
	brightness_to_set = (u8)(buf[0] - '0');
	buf1 = brightness[brightness_to_set];
	pr_info("[%s,%d]brightness level:%hhu, code: %x\n", __func__, __LINE__, brightness_to_set, buf1);

	/* write bank */
	msgs[0].addr = 0x48 >> 1;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &buf1;
		
	ret = i2c_transfer(tm1650_info.client->adapter, &(msgs[0]), 1);
	if (ret < 0)
	{
		pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
		return -1;
	}
	return count;
}
				 
static DEVICE_ATTR(first, 0600, NULL, set_first);
static DEVICE_ATTR(second, 0600, NULL, set_second);
static DEVICE_ATTR(third, 0600, NULL, set_third);
static DEVICE_ATTR(fourth, 0600, NULL, set_fourth);
static DEVICE_ATTR(brightness, 0600, NULL, set_brightness);

static struct attribute *tm1650_attrs[] = {
    &dev_attr_first.attr,
    &dev_attr_second.attr,
    &dev_attr_third.attr,
	&dev_attr_fourth.attr,
	&dev_attr_brightness.attr,
    NULL
};

static const struct attribute_group tm1650_attrs_group = {
    .attrs = tm1650_attrs,
};

static const struct attribute_group *tm1650_attr_groups[] = {
    &tm1650_attrs_group,
    NULL
};

static const struct file_operations tm1650_fops = {
	.open = tm1650_open,
	.release = tm1650_release,
    .compat_ioctl = tm1650_compat_ioctl,
};

static struct miscdevice tm1650_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = TM1650_NAME,
	.fops = &tm1650_fops,
	.groups = tm1650_attr_groups,
};

static int tm1650_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret = 0;
	pr_info("[%s,%d]match tm1650", __func__, __LINE__);
	tm1650_info.client = client;
	
	#if 1
	
	{
		struct i2c_msg msgs[5];
		u8 buf1 = 0x01;
		u8 buf2 = 0xbf;
		u8 buf3 = 0xbf;
		u8 buf4 = 0xbf;
		u8 buf5 = 0xbf;

		/* write bank */
		msgs[0].addr = 0x48 >> 1;
		msgs[0].flags = 0;
		msgs[0].len = 1;
		msgs[0].buf = &buf1;
		
		ret = i2c_transfer(client->adapter, &(msgs[0]), 1);
		if (ret < 0)
		{
			pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
			return ret;
		}
		
		// 0x68 0x6a 0x6c 0x6e
		/* write reg addr */
		msgs[1].addr = 0x68 >> 1;
		msgs[1].flags = 0;
		msgs[1].len = 1;
		msgs[1].buf = &buf2;
		ret = i2c_transfer(client->adapter, &(msgs[1]), 1);
		if (ret < 0)
		{
			pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
			return ret;
		}
		
		/* read data */
		msgs[2].addr = 0x6a >> 1;
		msgs[2].flags = 0;
		msgs[2].len = 1;
		msgs[2].buf = &buf3;
		
		ret = i2c_transfer(client->adapter, &(msgs[2]), 1);
		if (ret < 0)
		{
			pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
			return ret;
		}
		
		msgs[3].addr = 0x6c >> 1;
		msgs[3].flags = 0;
		msgs[3].len = 1;
		msgs[3].buf = &buf4;
		
		ret = i2c_transfer(client->adapter, &(msgs[3]), 1);
		if (ret < 0)
		{
			pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
			return ret;
		}
		
		msgs[4].addr = 0x6e >> 1;
		msgs[4].flags = 0;
		msgs[4].len = 1;
		msgs[4].buf = &buf5;
		ret = i2c_transfer(client->adapter, &(msgs[4]), 1);
		if (ret < 0)
		{
			pr_info("[%s,%d]i2c_transfer failed, code: %d\n", __func__, __LINE__, ret);
			return ret;
		}
	}
	#endif
	
	ret = misc_register(&tm1650_misc_device);
	if (ret < 0)
	{
		pr_info("[%s,%d]register misc driver failed\n", __func__, __LINE__);
		return ret;
	}
	pr_info("[%s,%d]register misc driver sucess\n", __func__, __LINE__);
	return 0;
}

static int tm1650_remove(struct i2c_client *client)
{
	pr_info("[%s,%d]remove tm1650", __func__, __LINE__);
	tm1650_info.client = NULL;
	misc_deregister(&tm1650_misc_device);
	return 0;
}

static struct i2c_driver tm1650_i2c_driver = {
	.driver = {
		.name = TM1650_NAME,
		.of_match_table = of_match_ptr(imx258_of_match),
	},
	.probe		= &tm1650_probe,
	.remove		= &tm1650_remove,
	.id_table	= tm1650_id,
};



static int __init tm1650_init(void)
{
	return i2c_add_driver(&tm1650_i2c_driver);
}

static void __exit tm1650_exit(void)
{
	i2c_del_driver(&tm1650_i2c_driver);
}

module_init(tm1650_init);
module_exit(tm1650_exit);
MODULE_DESCRIPTION("tm1650 i2c driver");
MODULE_LICENSE("GPL v2");
