
void image2D::esperance_max(const int k, const char* filename) {
    image2D image = image2D(filename);
    double N = image.getheight() * image.getwidth();
    image.save("D:/image_debut.bmp");
    Mat_<float> raw(N, 3);
    raw = 0;
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            raw.at<float>(i * width + j, 0) = (float)img[i][j][0] / 255;
        }
    }
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            raw.at<float>(i * width + j, 1) = (float)img[i][j][1] / 255;
        }
    }
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            raw.at<float>(i * width + j, 2) = (float)image.img[i][j][2] / 255;
        }
    }
    int tab[4] = {};
    for (int i = 0; i < k; i++) {
        tab[i] = my_rand(N);
        std::cout << "Valeur tirees : " << tab[i] << std::endl;
    }
    Mat_<float> u(k, 3);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < 3; j++) {
            u.at<float>(i, j) = raw.at<float>(tab[i], j);
        }
    }
    std::cout << "Valeur de u : " << u << std::endl;
    Mat_<float> raw_tmp(N / k, 1);
    Mat mean, stdv;
    Mat_<float> v(1, k);
    for (int j = 0; j < k; j++) {
        for (int i = 0; i < N - k; i = i + k) {
            raw_tmp.at<float>(i / k, 0) = raw.at<float>(i + j, 0);
        }
        meanStdDev(raw_tmp, mean, stdv);
        v.at<float>(0, j) = stdv.at<double>(0);
    }
    std::cout << "Valeur de v :" << v << std::endl;
    Mat_<float> w(k, 1);
    w = (1. / k);
    Mat_<float> u0(k, 3);
    u0 = 0;
    Mat_<float> v0(1, k);
    v0 = 0;
    Mat_<float> w0(k, 1);
    w0 = 0;
    std::cout << "w : " << w << std::endl;
    int iteration = 1;
    float energy = sum(u.mul(u))[0] + sum(v.mul(v))[0] + sum(w.mul(w))[0];
    std::cout << "energy : " << energy << std::endl;
    Mat_<float> x_u(N, 3);
    x_u = 0;
    Mat_<float> ones(N, 1);
    ones = 1;
    Mat_<float> psum(N, 1);
    psum = 0;
    Mat_<float> psum2(1, k);
    psum2 = 0;
    Mat_<float> psum3(1, k);
    psum3 = 0;
    Mat_<float> p(N, k);
    p = 0;
    Mat_<float> pnorm(N, k);
    pnorm = 0;
    Mat_<float> x_uSum(N, 1);
    x_uSum = 0;
    Mat_<float> pnormt(k, N);
    pnormt = 0;
    do {
        // calcul des propabilité des membres
        for (int jj = 0; jj < k; jj++) {
            for (int ss = 0; ss < 3; ss++) {
                for (int ii = 0; ii < N; ii++) {
                    x_u.at<float>(ii, ss) = raw.at<float>(ii, ss) - u.at<float>(jj, ss);
                }
            }
            x_u = x_u.mul(x_u);
            for (int ii = 0; ii < N; ii++) {
                p.at<float>(ii, jj) = pow(sqrt(2 * 3.141592653589793235) * v.at<float>(0, jj), -3) * exp((-1. / 2) * sum(x_u.row(ii))[0] / pow(v.at<float>(0, jj), 2));
                p.at<float>(ii, jj) = w.at<float>(jj, 0) * p.at<float>(ii, jj);
            }
        }
        std::cout << "Propabilite des membres calculees.  " << std::endl;
        // normalisation en x
        for (int jj = 0; jj < N; jj++) {
            for (int ss = 0; ss < k; ss++) {
                psum.at<float>(jj) += p.at<float>(jj, ss);
            }
        }

        for (int jj = 0; jj < N; jj++) {
            for (int ss = 0; ss < k; ss++) {
                p.at<float>(jj, ss) = p.at<float>(jj, ss) / psum.at<float>(jj);
            }
        }
        // normalisation en y
        for (int jj = 0; jj < N; jj++) {
            for (int ss = 0; ss < k; ss++) {
                psum2.at<float>(0, ss) += p.at<float>(jj, ss);
            }
        }

        for (int jj = 0; jj < N; jj++) {
            for (int ss = 0; ss < k; ss++) {
                pnorm.at<float>(jj, ss) = p.at<float>(jj, ss) / psum2.at<float>(0, ss);
            }
        }
        u0 = u; v0 = v; w0 = w;
        pnormt = pnorm.t();
        u = pnormt * raw;
        //std::cout << "u : " <<u<< std::endl; 
        for (int jj = 0; jj < k; jj++) {
            for (int ss = 0; ss < 3; ss++) {
                for (int ii = 0; ii < N; ii++) {
                    x_u.at<float>(ii, ss) = pow(raw.at<float>(ii, ss) - u.at<float>(jj, ss), 2);
                }
                reduce(x_u, x_uSum, 1, REDUCE_SUM);
                Mat valeur = (pnormt.row(jj) * x_uSum);
                v.at<float>(jj) = sqrt(0.333 * valeur.at<float>(0));
            }
        }
        //
        std::cout << "iteration : " << iteration << std::endl;
        for (int jj = 0; jj < N; jj++) {
            for (int ss = 0; ss < k; ss++) {
                psum3.at<float>(0, ss) += p.at<float>(jj, ss);
            }
        }
        w = psum3 / N;
        w = w.t();
        std::cout << w << std::endl;
        psum2 = 0;
        psum = 0;
        psum3 = 0;
        iteration = iteration + 1;
        float energy_u = 0;
        float energy_v = 0;
        float energy_w = 0;
        for (int jj = 0; jj < k; jj++) {
            for (int ss = 0; ss < 3; ss++) {
                energy_u += pow(u.at<float>(jj, ss) - u0.at<float>(jj, ss), 2);
            }
        }
        for (int jj = 0; jj < k; jj++) {
            energy_v += pow(v.at<float>(0, jj) - v0.at<float>(0, jj), 2);
        }
        for (int jj = 0; jj < k; jj++) {
            energy_w += pow(w.at<float>(jj) - w0.at<float>(jj), 2);
        }
        energy = energy_u + energy_v + energy_w;
        std::cout << "Energy : " << energy << std::endl;
    } while (energy > 0.0001);

    Mat_<float> imgRe(N, 3);
    Mat fin_img;
    Mat_<float> imgRecopy(N, 1);
    Mat_<float> imgR(height, width);
    Mat_<float> imgG(height, width);
    Mat_<float> imgB(height, width);
    Mat kColor = u;
    std::vector<Mat> channels;
    imgRe = p * kColor;

    imgRecopy = imgRe.col(0);
    for (int ii = 0; ii < height; ii++) {
        for (int jj = 0; jj < width; jj++) {
            imgR.at<float>(jj, width - ii - 1) = imgRecopy.at<float>(ii + jj * width);
        }
    }
    imgRecopy = imgRe.col(1);
    for (int ii = 0; ii < height; ii++) {
        for (int jj = 0; jj < width; jj++) {
            imgG.at<float>(jj, width - ii - 1) = imgRecopy.at<float>(ii + jj * width);
        }
    }
    std::cout << "bon" << std::endl;
    imgRecopy = imgRe.col(2);
    for (int ii = 0; ii < height; ii++) {
        for (int jj = 0; jj < width; jj++) {
            imgB.at<float>(jj, width - ii - 1) = imgRecopy.at<float>(ii + jj * width);
        }
    }

    for (int ii = 0; ii < height; ii++) {
        for (int jj = 0; jj < width; jj++) {
            image.img[jj][width - ii - 1][0] = imgR.at<float>(jj, ii) * 255;
        }
    }
    for (int ii = 0; ii < height; ii++) {
        for (int jj = 0; jj < width; jj++) {
            image.img[jj][width - ii - 1][1] = imgG.at<float>(jj, ii) * 255;
        }
    }
    for (int ii = 0; ii < height; ii++) {
        for (int jj = 0; jj < width; jj++) {
            image.img[jj][width - ii - 1][2] = imgB.at<float>(jj, ii) * 255;
        }
    }
    image.save("D:/image_fin.bmp");
}
