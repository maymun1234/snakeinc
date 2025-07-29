
//tamam bir metod olsun. çağrıldığında bir txt dosyasıan ekleme yapcak

#include "stats.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>


#include "hud.h"
#include "automated.h"

int foodid =0; // Başlangıçta 1 olarak ayarlanır
    // Global veya static global

    // Global veya static global
ULONGLONG levelTime = 0;  // Global veya static global
void asavescoretotext( int score11) {
    //metodda sorun var: 
    // Dosyayı aç veya oluştur
    // Dosya yolu ve adı
    // Dosya yolu ve adıfchcfhcf
    // Dosya açma işlemi
fprintf(stdout, "Score: %d\n", score11);
    FILE* file = fopen("source/resources/scoretab.txt", "a"); 
    fprintf(stdout, "Score: %d\n\n", score11);
    //dosyanın son satırı foodeat ilebaşlıoyrsa o saatırı sil
    //beceremedim

    // Dosya açma kontrolü
    if (file == NULL) {
        fprintf(stderr, "Dosya açılamadı!\n");
        return;
    }
    // dosyaya 1 yaz
    if(score11 != 0){
         fprintf(file, "direction= %s\n", getDirectionName(currentdireciton));
        fprintf(file, "levelduration= %llu\n", levelTime); // foodid'yi dosyaya yaz
    // fprintf(file, "Skor dosyaya kaydedildi: %d \n",  currentdireciton);
        fprintf(file, "score=%d\n\n", score11); // Skoru dosyaya yaz
        fclose(file); // Dosyayı kapat
        foodid = 0; // Skor kaydedildikten sonra foodid sıfırlanır
        levelTime = 0; // Level süresi sıfırlanır
        fprintf(file, "LEVELDATASTART\n\n"); // foodid'yi dosyaya yaz
            return;
        
        }
    
  
 
    
}

ULONGLONG foodStartTime = 0;  // Global veya static global
    // Global veya static global
void foodeatdurationms(int iseated){
    if (iseated == 0) {
        foodStartTime = GetTickCount64();  // Başlangıç zamanı saklanır
        
    } else {
        FILE* file = fopen("source/resources/scoretab.txt", "a"); 
        if (file == NULL) {
            fprintf(stderr, "Dosya açılamadı!\n");
            return;
        }
        foodid++;
        ULONGLONG duration = GetTickCount64() - foodStartTime;
        levelTime += duration;  // Toplam süreye eklenir
        fprintf(file, "foodeated %d = %llu ms\n", foodid, duration);
        fclose(file);
    }
}

