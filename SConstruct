var = ARGUMENTS.get('useropts', 'testmacro')
print(var)

env = Environment()
AddOption(
	'--project',
	dest='project',
	type='string',
	nargs=1,
	action='store',
	default='macro',
    metavar='PROJECT',
	help='project: led_blink uart_test i2c_test',
)

env = Environment(PROJECT=GetOption('project'))
print(GetOption('project'))

print("This is a test!")
print(COMMAND_LINE_TARGETS)
if 'led_blink' in COMMAND_LINE_TARGETS:
	print("Don't forget to copy `bar' to the archive!")

