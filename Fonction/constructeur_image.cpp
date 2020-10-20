image2D::image2D(const char* path) {
    FILE* f;
    errno_t err;
    err=fopen_s(&f, path, "rb");
    
    unsigned char header[54];
    (*this).dy = 1;
    (*this).dx = 1;

    if (err==0) {
        std::cout << "File open" << std::endl;
        fread(header, sizeof(unsigned char), 54, f); // read the 54-byte header
        // extract image height and width from header
        int width = *(int*)&header[18];
        int height = *(int*)&header[22];
        int size = 3 * width * height;
        (*this).width = width;
        (*this).height = height;
        (*this).sz = width * height;
        this->imalloc();

        unsigned char* image = new unsigned char[size]; // allocate 3 bytes per pixel
        fread(image, sizeof(unsigned char), size, f);//We fill the FULL image
        fclose(f);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < 3; k++) {

                    img[width-i-1][height-j-1][k] = image[3 *(i * width + j) + k];
                }
            }
        }

        /*
        for (int i=0; i<(*this).height; i++) {
            for (int j=0; j<(*this).width; j++) {
                double mean=0;
                mean=((double)imageRED[i*((*this).width)+j]+(double)imageBLU[i*((*this).width)+j]+(double)imageGRE[i*((*this).width)+j])/3.0;
                img[i][j][0]=imageRED[i*width+j]
                this->img[j][i]=(int)mean;
            }
        }
        */
        delete[] image;
        image = NULL;
    }
    else std::cout << "Error open file" << std::endl;
}