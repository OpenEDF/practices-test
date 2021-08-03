static int array[8] = {0x01, 0x02, 0x03, 0x04};

int addr(void)
{
	return (array[0] + array[1]);
}
