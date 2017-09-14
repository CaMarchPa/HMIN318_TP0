#include <iostream>
#include "CImg.h"

using namespace cimg_library;

// float* showInformation(CImg<float> img);

int main(int argc, char **argv) {
    /* code */
    //1 - lire l'image format Analyze
    if (argc != 2){
        std::cout << "usage : ./fichier.exe nom_image.hdr" << std::endl;
        return -1;
    }

    CImg<float> img;
    float voxelsize[3];
    img.load_analyze(argv[1], voxelsize);

    //1 -
    // float *size;
    // size = showInformation(img);
    // float x = size[0];
    // float y = size[1];
    // float z = size[2];
    //Afficher la taille de l'image, le vox min et max
    float x = img.width();
    float y = img.height();
    float z = img.depth();

    float & min = img.min();
    float & max = img.max();
    std::cout << "Img : x = " << x << ", y = " << y << ", z = " << z << std::endl;

    std::cout << "la valeur du pixel minimum : " << min << std::endl;
    std::cout << "la valeur du pixel maximum : " << max << std::endl;
    //Afficher la valeur du voxel
    std::cout << "La dimension du voxel : " << img.linear_atXYZ(256, 256, 12, 0) << std::endl;

    std::cout << "/* message le x ==> */" << x <<'\n';

    //2 - Visualisation d'une image 3D
    //afficher l'image à l'écran
    CImg<float> img2d = img.get_projections2d(x, y, z);
    // CImg<unsigned char> img2(512, 512, 1, 3, 1);

    CImgDisplay img_disp(img, "test"); // afficher l'image en coupes axiales
    CImgDisplay img_dispMPR(img2d, "MPR");
    int displaySlice = 0;
    bool redraw = false;

    // while((!img_disp.is_closed() && !img_dispMPR.is_closed()) && img_disp.is_keyESC() && img_dispMPR.is_keyESC())
    while(!img_disp.is_closed() && !img_disp.is_keyESC()) {
        img_dispMPR.wait();
        if (img_dispMPR.is_resized()) {
            img_dispMPR.resize();
        }

        if(img_dispMPR.wheel()) {
            const int scroll = img_dispMPR.wheel();
            displaySlice = displaySlice+scroll;
            if (displaySlice < 0) {
                displaySlice = 0;
            } else {
                if (displaySlice>=(int)z) {
                    displaySlice = (int)z - 1;
                }
            }
            img_dispMPR.set_wheel();
            redraw = true;
        }

        if (redraw) {
            CImg<> mpr_img = img.get_slice(displaySlice);
            mpr_img.resize(512, 512);
            img_dispMPR.display(mpr_img);
            redraw = false;
        }

        if (img_dispMPR.is_keyM()) {
            CImg<> mpr_img = img.blur_median(7, 0);
            img_dispMPR.display(mpr_img);
        }
        // img_disp.wait();
    }
    return 0;
}

// float* showInformation(CImg<float> img){
//     //Afficher la taille de l'image, le vox min et max
//     float x = img.width();
//     float y = img.height();
//     float z = img.depth();
//     float size[3] = {x, y, z};
//
//     float & min = img.min();
//     float & max = img.max();
//     std::cout << "Img : x = " << x << ", y = " << y << ", z = " << z << std::endl;
//
//     std::cout << "la valeur du pixel minimum : " << min << std::endl;
//     std::cout << "la valeur du pixel maximum : " << max << std::endl;
//     //Afficher la valeur du voxel
//     std::cout << "La dimension du voxel : " << img.linear_atXYZ(256, 256, 12, 0) << std::endl;
//     return size;
// }
