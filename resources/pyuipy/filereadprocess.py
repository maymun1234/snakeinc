import os

# Geçerli dosyanın bulunduğu klasörü al
current_dir = os.path.dirname(__file__)
file_path = os.path.join(current_dir, "scoretab.txt")

def cleanfile():
    with open(file_path, 'r') as f:
        lines = f.readlines()

    
    stop_index = len(lines) 
    for i in reversed(range(len(lines))):
        line = lines[i].strip()
        if line.startswith("fd"):
            print(f"Siliniyor: {line}")
            stop_index = i  # Bu satırı sil
        else:
            break  # İlk fd olmayan satırda dur

    # Yeni içerik: baştan stop_index'e kadar olan satırlar
    new_lines = lines[:stop_index]

    # Dosyayı güncelle
    with open(file_path, 'w') as f:
        f.writelines(new_lines)

    print("Sondaki 'fd' satırları silindi.")

print("scoretab.txt okunuyor...")
cleanfile()
