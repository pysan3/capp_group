import numpy as np
from numpy.core.defchararray import count

file_names = ['bump_a', 'bump_b']
height = 256
width = 256

for file in reversed(file_names):
    with open(f'{file}.raw', 'rb') as f:
        data = f.read()
        img = np.array([d for d in data], dtype=np.uint8).reshape((height, width))
        print(img.shape)
        new_img = np.hstack([img, np.fliplr(img)])
        print(new_img.shape)
        new_img = np.vstack([new_img, np.flipud(new_img)])
        print(new_img.shape)
        with open(f'new_{file}.raw', 'wb') as n:
            n.write(new_img.flatten())