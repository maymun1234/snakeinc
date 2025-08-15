import sys
import os
from PyQt5.QtWidgets import QApplication, QMainWindow
from PyQt5 import uic
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from pathlib import Path




class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
    
        ui_path = os.path.join(os.path.dirname(__file__), 'maineditorqt.ui')
        uic.loadUi(ui_path, self)
        self.modeselector_widget = self.findChild(QComboBox, 'modeselector')
        self.modeselector_widget.currentTextChanged.connect(self.on_combobox_changed)
        self.playbutton = self.findChild(QPushButton, "playbutton")
        self.speedslider = self.findChild(QSlider, "speeddiameter")
        self.speedslider.valueChanged.connect(self.onsppedsliderchanged)
        self.playbutton.clicked.connect(self.on_play_button_clicked)
        self.label_3 = self.findChild(QLabel, "frogimage")  # .ui'deki QLabel ismi
       


        png_path = os.path.join(os.path.dirname(__file__), "frog.png")

        # Görseli yükle ve QLabel'e ayarla
        pixmap = QPixmap(png_path)
        self.label_3.setPixmap(pixmap)
        self.label_3.setScaledContents(True)  # Otom
       # self.playbuttonmu

    
        


    def loadcombobox(self):
        filename = Path("../source/automated.h").resolve()

        with open(filename, "r") as file:
            dizi = file.readlines()

        for i in range(4):
            temiz_satir = dizi[i + 11].strip().strip(",")
            print(temiz_satir)
            temiz_satir = temiz_satir.lower()
            self.modeselector_widget.addItem(temiz_satir)

       
        satir_index = 23
        satir = dizi[satir_index]

        if len(satir) > 36:
            aktif_mod = satir[36:].strip().strip(';')
            print("Aktif mod:", aktif_mod)
            


            # ComboBox i
            index = self.modeselector_widget.findText(aktif_mod.lower())
            print(aktif_mod.lower())
            if index != -1:
                self.modeselector_widget.setCurrentIndex(index)


        



    def on_combobox_changed(self, text):
        print(f"Seçilen mod: {text}")
        
        filename = Path("../source/automated.h").resolve()

        try:
            with open(filename, "r") as file:
                dizi = file.readlines()

            # 26. satırı al (Python index: 25)
            satir_index = 23
            satir = dizi[satir_index]

            
            if len(satir) < 37:
                satir = satir.rstrip("\n") + " " * (37 - len(satir)) + "\n"

            yeni_satir = satir[:36] + text.upper() + ";\n"
            dizi[satir_index] = yeni_satir
            global automationtype 
            automationtype = satir_index
        
            with open(filename, "w") as file:
                file.writelines(dizi)

            print("automated.h dosyası güncellendi.")

        except Exception as e:
            print(f"Hata oluştu: {e}")

        
    def on_play_button_clicked(self):
        print("ksknvdsnksvdnk")

    
    def onfrogclicked(self):
        print("lsfddlsdj")

    def onsppedsliderchanged(self):
        print("Slider değişti")

        filename = Path("../source/automated.c").resolve()

        # Dosyayı oku
        with open(filename, "r") as file:
            dizi = file.readlines()

        # Mevcut slider değerini al
        value = self.speedslider.value()
        print("Slider değeri:", value)
        print(automationtype)
        print(23)
            # 277. satırı düzenle
                
        if(1):
            satir_index = 15 # Python index 0'dan başlar
            satir = dizi[satir_index]

            yeni_satir = satir[:17] + f"{value};\n"

            dizi[satir_index] = yeni_satir

                # Güncelle
            dizi[satir_index] = yeni_satir

                # Dosyayı tekrar yaz
            with open(filename, "w") as file:
                file.writelines(dizi)

                print("277. satır güncellendi.")  

        else:
            print("nsdnmdsmnfv"+automationtype)
            print("fhnng")

        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    window.loadcombobox()
    sys.exit(app.exec_())



