#hud.c dosyasından fpsfark değişeknini alıp ui ile ekrana yazdıran pytho nkod
# -*- coding: utf-8 -*-
# This file is part of the OpenGL_Test project.
import os
import sys
import ctypes
import ctypes
from ctypes import wintypes
import time


#hud.c dosyasını yükle
hud_c_path = os.path.join(os.path.dirname(__file__), 'hud.c')
#fpsfark değişkenini çek
def get_fpsfark():
    # C kodunu çalıştırmak için ctypes kullan
    hud_c = ctypes.CDLL(hud_c_path)
    
    # fpsfark değişkenini al
    hud_c.get_fpsfark.restype = ctypes.c_int
    fpsfark = hud_c.get_fpsfark()
    
    return fpsfark


get_fpsfark.restype = ctypes.c_int
# Fonksiyonu çağır ve sonucu yazdır
if __name__ == "__main__":
    fpsfark = get_fpsfark()
    print(f"FPS Fark: {fpsfark}")



