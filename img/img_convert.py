from PIL import Image
import numpy as np

im = Image.open('background.jpg').convert('RGB')

print(im.size)
data = []
for i in range(512):
    for j in range(im.size[1]):
        print(im.getpixel((i+64, j)))
        data.append(im.getpixel((i+64, j)))

arr = np.array(data + [(255,255,255) for _ in range((512-im.size[1])*512)], dtype=np.uint8)
print(arr)
with open('background.raw', 'wb') as f:
    f.write(np.array(data, dtype=np.uint8).flatten())