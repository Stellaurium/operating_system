import fileinput

TEMPLATE = '''
     AAAAAAAAA
    FF       BB
    FF       BB
    FF       BB
    FF       BB
     GGGGGGGG
   EE       CC
   EE       CC
   EE       CC
   EE       CC
    DDDDDDDDD
''' 

# \033[颜色码m 表示更改颜色
# \033[0m     表示恢复默认
# These are ANSI Escape Codes
CLEAR = '\033[2J\033[1;1f'  # Clear screen and move cursor to top-left
WHITE = '\033[37m░\033[0m'  # A hollow block (White)
BLACK = '\033[31m█\033[0m'  # A solid block  (Red)

for line in fileinput.input():
    # Execute the input line (like "A=0; B=1; ...") as Python code; the
    # variables A, B, ... will be stored in loaded_variable_dict.
    # exec 接受三个参数，后两个都可以逐个省略 第一个是代码 第二个是一个字典
    # 执行过程中创建的变量会存放在字典当中
    # loaded_variable_dict := {} 是在表达式中创建了一个变量 (就类似c++的初始化)，为了在后面还能够使用字典
    # 省去多开一行写 loaded_variable_dict = {}
    exec(line, (loaded_variable_dict := {}))

    # Initialize the display with a clear screen and the template.
    disp = CLEAR + TEMPLATE

    for ch in 'ABCDEFG':
        # Determine the block color.
        block = {
            0: WHITE,
            1: BLACK,
        }.get(loaded_variable_dict[ch], '?') # 字典的get函数

        # Replace each character in the template with its block.
        disp = disp.replace(ch, block)

    print(disp)
