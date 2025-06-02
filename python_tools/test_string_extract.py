#!/usr/bin/python python3
import os
import re

# 获取py文件所在文件夹绝对路径
pydir = os.path.dirname(os.path.realpath(__file__))

# 获取C文件所在文件夹相对py文件路径
cdir = os.path.join(os.path.dirname(pydir), "src")

# 文件路径
input_file = os.path.join(cdir, "strings.c")
output_file = os.path.join(os.path.dirname(pydir), "extracted_strings.txt")

# 正则表达式匹配变量名和 _("...") 之间的内容
pattern = re.compile(r'const\s+u8\s+(\w+)\[\]\s*=\s*_\("([^"]*)"\)')

# 读取文件并提取内容
with open(input_file, 'r', encoding='utf-8') as infile, open(output_file, 'w', encoding='utf-8') as outfile:
    for line in infile:
        match = pattern.search(line)
        if match:
            variable_name = match.group(1)
            text_content = match.group(2)
            outfile.write(f"{text_content}\t{variable_name}\n")

print(f"提取完成，结果已保存到 {output_file}")