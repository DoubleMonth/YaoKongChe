/*
 * �����嵥������һ�������豸���ղ��������ݵ�ʾ������
 * ���̵����� uart_dma_sample ��������ն�
 * ������ø�ʽ��uart_dma_sample uart2
 * ������ͣ�����ڶ���������Ҫʹ�õĴ����豸���ƣ�Ϊ����ʹ��Ĭ�ϵĴ����豸
 * �����ܣ�ͨ������ uart2 ����ַ���"hello RT-Thread!"����ͨ������ uart2 ����һ���ַ���������������ͨ�����ݽ�����ʹ�ÿ���̨��ʾ��Ч���ݡ�
*/

#include <rtthread.h>
#include <rtdevice.h>

#define SAMPLE_UART_NAME                 "uart2"
#define DATA_CMD_END                     '\r'       /* ����λ����Ϊ \r�����س��� */
#define ONE_DATA_MAXLEN                  20         /* ���������ݵ���󳤶� */

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* �������ݻص����� */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    if (size > 0)
    {
        rt_sem_release(&rx_sem);
    }
    return RT_EOK;
}

static char uart_sample_get_char(void)
{
    char ch;

    while (rt_device_read(serial, 0, &ch, 1) == 0)
    {
        rt_sem_control(&rx_sem, RT_IPC_CMD_RESET, RT_NULL);
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}

/* ���ݽ����߳� */
static void data_parsing(void)
{
    char ch;
    char data[ONE_DATA_MAXLEN];
    static char i = 0;

    while (1)
    {
        ch = uart_sample_get_char();
        rt_device_write(serial, 0, &ch, 1);
        if(ch == DATA_CMD_END)
        {
            data[i++] = '\0';
            rt_kprintf("data=%s\r\n",data);
            i = 0;
            continue;
        }
        i = (i >= ONE_DATA_MAXLEN-1) ? ONE_DATA_MAXLEN-1 : i;
        data[i++] = ch;
    }
}

static int uart_data_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread!\r\n";
	struct serial_configure config=RT_SERIAL_CONFIG_DEFAULT;
    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* ����ϵͳ�еĴ����豸 */
    serial = rt_device_find(uart_name);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
	config.baud_rate=BAUD_RATE_9600;
	config.data_bits=DATA_BITS_8;
	config.stop_bits=STOP_BITS_1;
	config.parity=PARITY_NONE;
	rt_device_control(serial,RT_DEVICE_CTRL_CONFIG,&config);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial, uart_rx_ind);
    /* �����ַ��� */
    rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* ���� serial �߳� */
    rt_thread_t thread = rt_thread_create("serial", (void (*)(void *parameter))data_parsing, RT_NULL, 1024, 25, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

/* ������ msh �����б��� */
MSH_CMD_EXPORT(uart_data_sample, uart device sample);