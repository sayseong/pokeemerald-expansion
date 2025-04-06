import os
import re
import pandas as pd

def convert_description_to_multiline(description):
    lines = description.strip().split('\n')
    return '        .description = COMPOUND_STRING(\n' + ''.join(
        f'            "{line}\\n"\n' for line in lines
    )[:-1] + '        ),'  # 去掉最后一行的 \n 再添加 ), 

def query_pokemon_info(name, df):
    try:
        result = df.loc[name]
        return {
            'speciesName': result['speciesName'],
            'categoryName': result['categoryName'],
            'description': result['description']
        }
    except KeyError:
        return None

def replace_species_info(content, df):
    pattern = r"\[SPECIES_(\w+)\]\s*=\s*\{.*?\n\s*\},?"
    matches = list(re.finditer(pattern, content, re.DOTALL))

    for match in matches:
        species = match.group(1)
        original_block = match.group(0)
        info = query_pokemon_info(species, df)
        if not info:
            print(f"❌ 未找到 {species}，跳过")
            continue

        block = original_block

        # 替换 .speciesName
        block = re.sub(r'\.speciesName\s*=\s*_\(.*?\),', f'.speciesName = _("{info["speciesName"]}"),', block)

        # 替换 .categoryName
        block = re.sub(r'\.categoryName\s*=\s*_\(.*?\),', f'.categoryName = _("{info["categoryName"]}"),', block)

        # 替换 .description
        block = re.sub(r'\.description\s*=\s*COMPOUND_STRING\((?:.|\n)*?\),', convert_description_to_multiline(info["description"]), block)

        content = content.replace(original_block, block)

    return content

if __name__ == "__main__":

    work_folder = r"D:\code\pokeemerald-expansion\src\data\pokemon\species_info"
    df = pd.read_excel(r'D:\code\pokeemerald-expansion\python_tools\src\图鉴对应翻译表手动调整.xlsx')
    df.set_index('name', inplace=True)

    for filename in os.listdir(work_folder):
        if re.match(r'^gen_\d+_families\.h$', filename):
            file_path = os.path.join(work_folder, filename)
            print(f"✅ 处理文件：{file_path}")
            with open(file_path, "r", encoding="utf-8") as f:
                content = f.read()

            new_content = replace_species_info(content, df)

            with open(file_path, "w", encoding="utf-8") as f:
                f.write(new_content)
