from PIL import Image
import numpy as np


class WorkImage:
    __image = 0
    __NpImage = 0

    def set_image(self, s):
        self.__image = Image.open(s)
        self.__NpImage = np.asarray(self.__image)

    def get_image_npmatrix(self):
        return self.__NpImage

    def get_canal_pixel(self, x, y, c):
        return self.__NpImage[x, y, c]

    def get_rgb_pixel(self, x, y):
        return self.__NpImage[x, y]
