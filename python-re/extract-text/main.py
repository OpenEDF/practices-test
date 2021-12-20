# extract info from text
import re 
def extract_info(file_path):
    linelists = []
    with open(file_path, 'r') as f:
        linelists = f.readlines()
    f.close()
    
    # search and collect
    info = []
    for index in range(len(linelists)):
        if ("MACCOREBASEADDRESS_BASEADDR" in linelists[index]):
            info.append(linelists[index])

    for index in range(len(linelists)):
        if ("MACPLBASEADDRESS_BASEADDR" in linelists[index]):
            info.append(linelists[index])

    for index in range(len(linelists)):
        if ("MACBCOREBASEADDRESS_BASEADDR" in linelists[index]):
            info.append(linelists[index])

    for index in range(len(linelists)):
        if ("MACBPLBASEADDRESS_BASEADDR" in linelists[index]):
            info.append(linelists[index])
    
    # extract hex from info list
    data = []
    for index in range(len(info)):
        data.extend(re.findall(r'0x[0-9A-F]+', info[index], re.I))
        data[index] = int(data[index], 16)
   
    # claculater offset
    offset = []
    offset.append(data[2] - data[0])
    offset.append(data[3] - data[1])

    # compare and return or exit
    if (offset[0] == offset[1]):
        return hex(offset[0])
    else:
        print("Error: The offset of Mac CORE and PL are not equal, Please check and redesign the script.")
        exit()

if __name__ == "__main__":
   data = extract_info("regmap.txt")
   print(data)
