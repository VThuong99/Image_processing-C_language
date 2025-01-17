# Image Processing in C Language
This repo showcases various image processing techniques implemented in C.
## Image Processing Techniques
### 1. DCT (Discrete Cosine Transform)
The `DCT` module contains an implementation of the Discrete Cosine Transform, a FFT algorithm for image. This algorithm used to calculate the 'frequency' of image pixels.
### 2. Sobel edge detection
The `Sobel edge detection` implements the Sobel operator to detect edges in images: using a kernel slide through the image and perform convolution.
### 3. Face Detection using edge detection
The `face_detection` module applies basic face recognition using vector-based feature extraction.  
The algorithm of feature generation: Load input image -> Edge detection (horizontal, +45 degree, vertical, -45 degree) -> Winner take all -> Thresholding -> Sum of each column -> Final feature
### 4. JPEG Baseline Encoder
The JPEG compression standard:
- DCT-based encoder: Source image -> DCT -> Quantizer -> Entropy encoder -> Compressed image data.
- DCT-based decoder: Compressed image data -> Entropy decoder -> Dequantizer -> IDCT -> Rescontructed image data.
### 5. Simple tracking
The `Simple tracking` using 3 image: bg (background), fig1 and fig2 is 2 image that show the object before and after moving.  
First, I will detect the object by compare it with the background. Then, the direction of the object will be calculated by the dx and dy value, that get by calculate the `L2 distance` of 2 object after and before.

