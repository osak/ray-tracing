Implementation of https://raytracing.github.io/books/RayTracingInOneWeekend.html

As the final image is too slow to generate as-is, I modified the original version to take a RNG seed and scanlines to render. Then run 8 processes concurrently on an EC2 c4.2xlarge instance, where each process renders 100 scanlines.

All images generated throughout the course: [images/all-images.md](images/all-images.md)