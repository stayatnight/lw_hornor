import os
import hashlib
import xml.etree.ElementTree as ET
import shutil
import zipfile
# 文件名
name='flashimage-ota-xz-v0.1.bin'
filename = 'D:/code/h_test/honor_magiclink_sdk/ln882h-custom-imou/build-yankon-release/bin/flashimage-ota-xz-v0.1.bin'
folder_name = 'full'
files_to_copy = ['D:/code/h_test/honor_magiclink_sdk/ln882h-custom-imou/build-yankon-release/bin/flashimage-ota-xz-v0.1.bin', 'filelist.xml']
zip_filename = 'full_archive.zip'

# 检查文件是否存在
if not os.path.isfile(filename):
    print(f"文件 {filename} 不存在。")
    exit()

# 计算文件的SHA-256哈希值
with open(filename, 'rb') as file:
    sha256_hash = hashlib.sha256()
    while True:
        data = file.read(4096)
        if not data:
            break
        sha256_hash.update(data)
    sha256_value = sha256_hash.hexdigest()

# 获取文件大小
file_size = os.path.getsize(filename)

# 创建XML元素
root = ET.Element("root")
files = ET.SubElement(root, "files")
file = ET.SubElement(files, "file")

spath = ET.SubElement(file, "spath")
spath.text = name

sha256 = ET.SubElement(file, "sha256")
sha256.text = sha256_value

size = ET.SubElement(file, "size")
size.text = str(file_size)
def indent(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i
indent(root)
# 创建并保存XML文件
tree = ET.ElementTree(root)
tree.write("filelist.xml", encoding="utf-8", xml_declaration=True)

if not os.path.exists(folder_name):
    os.makedirs(folder_name)

# 将文件复制到full文件夹中
for file in files_to_copy:
    shutil.copy(file, folder_name)
import re

# 读取C文件内容,获取版本号
with open('D:/code/h_test/honor_magiclink_sdk/demo/demo.c', 'r',encoding='utf-8') as file:
    content = file.read()

# 定义正则表达式模式来匹配版本号
# 假设版本号的格式为 "x.x.x.xxx"，其中x是数字
pattern = r'"\d+\.\d+\.\d+\.\d+"'

# 在文件内容中搜索匹配项
match = re.search(pattern, content)

if match:
    # 提取并打印版本号（去除双引号）
    version = match.group(0)[1:-1]  # 使用切片去除匹配结果中的双引号
    print(f"Found version: {version}")
else:
    print("Version not found.")
zip_filename=version+'_ota.zip'
# 创建zip文件并将full文件夹中的内容添加到其中
with zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED) as zipf:
    for root, dirs, files in os.walk(folder_name):
        for file in files:
            zipf.write(os.path.join(root, file), os.path.relpath(os.path.join(root, file), os.path.join(root, '..')))

# 如果需要，可以删除full文件夹（可选）
# shutil.rmtree(folder_name)

print(f"Files have been zipped into {zip_filename}")