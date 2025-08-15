import os



KEY = 124793477347343  
#şifreleelyici
def encrypt_file(dosyayolu):
    with open(dosyayolu, "rb") as file:
        data = file.read()

    encrypted_data = bytes([b ^ KEY for b in data])
    
    encrypted_path = dosyayolu + ".enc"
    with open(encrypted_path, "wb") as file:
        file.write(encrypted_data)

    print(f"File encrypted and saved as: {encrypted_path}")
    return encrypted_path

def decrypt_file(dosyayolu):
    if not dosyayolu.endswith(".enc"):
        print("This file does not seem to be encrypted.")
        return

    with open(dosyayolu, "rb") as file:
        encrypted_data = file.read()

    decrypted_data = bytes([b ^ KEY for b in encrypted_data])
    
    original_path = dosyayolu[:-4]  # .enc uzantısını kaldır
    with open(original_path, "wb") as file:
        file.write(decrypted_data)

    print(f"File decrypted and saved as: {original_path}")
    return original_path
