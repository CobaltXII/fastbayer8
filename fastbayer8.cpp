#include <cstdio>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

unsigned char* fastbayer8(unsigned char* img, int w, int h) {
	#define C(x) (unsigned char)((float(x + 1) / 65.0f) * 255)
	const unsigned char dm8[8][8] = {
		{C(0 ), C(48), C(12), C(60), C(3 ), C(51), C(15), C(63)},
		{C(32), C(16), C(44), C(28), C(35), C(19), C(47), C(31)},
		{C(8 ), C(56), C(4 ), C(52), C(11), C(59), C(7 ), C(55)},
		{C(40), C(24), C(36), C(20), C(43), C(27), C(39), C(23)},
		{C(2 ), C(50), C(14), C(62), C(1 ), C(49), C(13), C(61)},
		{C(34), C(18), C(46), C(30), C(33), C(17), C(45), C(29)},
		{C(10), C(58), C(6 ), C(54), C(9 ), C(57), C(5 ), C(53)},
		{C(42), C(26), C(38), C(22), C(41), C(25), C(37), C(21)}
	};
	#undef C
	if (!img) {
		return nullptr;
	}
	unsigned char* o = new unsigned char[w * h * 3];
	#pragma omp parallel for
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			unsigned char* p = img + (j * w + i) * 3;
			unsigned char* op = o + (j * w + i) * 3;
			unsigned char c = dm8[i % 8][j % 8];
			op[0] = p[0] < c ? 0 : 255;
			op[1] = p[1] < c ? 0 : 255;
			op[2] = p[2] < c ? 0 : 255;
		}
	}
	return o;
}

char* strfmt(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	size_t bufsz = vsnprintf(NULL, 0, fmt, args) + 1;
	va_end(args);
	char* buf = (char*)malloc(bufsz);
	if (!buf) {
		return NULL;
	}
	va_start(args, fmt);
	vsnprintf(buf, bufsz, fmt, args);
	va_end(args);
	return buf;
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int w;
	int h;
	unsigned char* data = stbi_load(argv[1], &w, &h, NULL, STBI_rgb);
	if (!data) {
		fprintf(stderr, "could not load image %s (stbi_load)\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	data = fastbayer8(data, w, h);
	char* out = strfmt("fd8_%s", argv[1]);
	if (!stbi_write_png(strfmt(out, argv[1]), w, h, 3, data, w * 3)) {
		fprintf(stderr, "could not save image %s (stbi_write_png)\n", out);
		exit(EXIT_FAILURE);
	}
	stbi_image_free(data);
	return 0;
}