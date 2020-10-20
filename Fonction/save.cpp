void image2D::save(const char* fdest) {

    int fsize = 54 + 3 * width * height;  //w is your image width, h is image height, both int

    unsigned char* data = (unsigned char*)malloc(3 * width * height);
    memset(data, 0, 3 * width * height);

    int x, y;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            x = i; y = (height - 1) - j;
            for (int k = 0; k < 3; k++) {
                data[(y + x * width) * 3 + k] = (unsigned char)img[i][j][k];
            }
        }
    }

    unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };   //BMP header, voir la doc bmp
    bmpfileheader[0] = (unsigned char)('B');       //magic char 'BM'
    bmpfileheader[1] = (unsigned char)('M');
    bmpfileheader[2] = (unsigned char)(fsize);//taille du fichier
    bmpfileheader[3] = (unsigned char)(fsize >> 8);
    bmpfileheader[4] = (unsigned char)(fsize >> 16);
    bmpfileheader[5] = (unsigned char)(fsize >> 24);
    bmpfileheader[10] = (unsigned char)(54);        //size(headerinfo)+size(fileinfo)

    unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
    bmpinfoheader[0] = (unsigned char)(40);// size(headerinfo)
    bmpinfoheader[4] = (unsigned char)(width);//largeur
    bmpinfoheader[5] = (unsigned char)(width >> 8);
    bmpinfoheader[6] = (unsigned char)(width >> 16);
    bmpinfoheader[7] = (unsigned char)(width >> 24);
    bmpinfoheader[8] = (unsigned char)(height);//hauteur
    bmpinfoheader[9] = (unsigned char)(height >> 8);
    bmpinfoheader[10] = (unsigned char)(height >> 16);
    bmpinfoheader[11] = (unsigned char)(height >> 24);
    bmpinfoheader[12] = (unsigned char)(1);
    bmpinfoheader[14] = (unsigned char)(24);// bits/pixel

    unsigned char pad[3] = { 0,0,0 };                        //padding (cas image 24 bit)
    int padsz = (4 - (width * 3) % 4) % 4;                          // cf doc bmp pr la taille du padding

    FILE* f;
    errno_t err;
    err = fopen_s(&f, fdest, "wb");
    if (err==1) { printf("Error : can't open file"); exit(0); }
    /*
    for (int i=0;i<sizeof(bmpfileheader);i++){ // autre methode pour écrire
            putc(bmpinfoheader[i],f);
    }
*/
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for (int i = 0; i < height; i++) {
        fwrite(data + (width * (height - i - 1) * 3), 3, width, f);  //ajoute la padding après chaque colonne de data[];
        fwrite(pad, 1, padsz, f);
    }

    fclose(f);
    free(data);

}

