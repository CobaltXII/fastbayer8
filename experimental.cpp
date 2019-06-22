unsigned char* fastbayer27(unsigned char* img, int w, int h) {
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
			
			op[0] = p[0] < 127 ? (p[0] > c / 2) * 127 : 127 + (p[0] > 127 + c / 2) * 127;
			op[1] = p[1] < 127 ? (p[1] > c / 2) * 127 : 127 + (p[1] > 127 + c / 2) * 127;
			op[2] = p[2] < 127 ? (p[2] > c / 2) * 127 : 127 + (p[2] > 127 + c / 2) * 127;
		}
	}
	return o;
}

unsigned char* fd8fsed(unsigned char* cimg, int w, int h) {
	if (!cimg) {
		return nullptr;
	}
	int* img = new int[w * h * 3];
	for (int i = 0; i < w * h * 3; i++) {
		img[i] = cimg[i];
	}
	unsigned char* o = new unsigned char[w * h * 3];
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			int* p = img + (j * w + i) * 3;
			unsigned char* op = o + (j * w + i) * 3;
			int nr = op[0] = p[0] < 127 ? 0 : 255;
			int ng = op[1] = p[1] < 127 ? 0 : 255;
			int nb = op[2] = p[2] < 127 ? 0 : 255;
			int er = p[0] - nr;
			int eg = p[1] - ng;
			int eb = p[2] - nb;
			if (i == 0) {
				if (j == h - 1) {
					img[(j * w + i + 1) * 3 + 0] += er * 7 / 16;
					img[(j * w + i + 1) * 3 + 1] += eg * 7 / 16;
					img[(j * w + i + 1) * 3 + 2] += eb * 7 / 16;
				} else {
					img[(j * w + i + 1) * 3 + 0] += er * 7 / 16;
					img[(j * w + i + 1) * 3 + 1] += eg * 7 / 16;
					img[(j * w + i + 1) * 3 + 2] += eb * 7 / 16;
					img[((j + 1) * w + i) * 3 + 0] += er * 5 / 16;
					img[((j + 1) * w + i) * 3 + 1] += eg * 5 / 16;
					img[((j + 1) * w + i) * 3 + 2] += eb * 5 / 16;
					img[((j + 1) * w + i + 1) * 3 + 0] += er / 16;
					img[((j + 1) * w + i + 1) * 3 + 1] += eg / 16;
					img[((j + 1) * w + i + 1) * 3 + 2] += eb / 16;
				}
			} else if (i == w - 1) {
				if (j < h - 1) {
					img[((j + 1) * w + i - 1) * 3 + 0] += er * 3 / 16;
					img[((j + 1) * w + i - 1) * 3 + 1] += eg * 3 / 16;
					img[((j + 1) * w + i - 1) * 3 + 2] += eb * 3 / 16;
					img[((j + 1) * w + i) * 3 + 0] += er * 5 / 16;
					img[((j + 1) * w + i) * 3 + 1] += eg * 5 / 16;
					img[((j + 1) * w + i) * 3 + 2] += eb * 5 / 16;
				}
			} else {
				if (j == h - 1) {
					img[(j * w + i + 1) * 3 + 0] += er * 7 / 16;
					img[(j * w + i + 1) * 3 + 1] += eg * 7 / 16;
					img[(j * w + i + 1) * 3 + 2] += eb * 7 / 16;
				} else {
					img[(j * w + i + 1) * 3 + 0] += er * 7 / 16;
					img[(j * w + i + 1) * 3 + 1] += eg * 7 / 16;
					img[(j * w + i + 1) * 3 + 2] += eb * 7 / 16;
					img[((j + 1) * w + i - 1) * 3 + 0] += er * 3 / 16;
					img[((j + 1) * w + i - 1) * 3 + 1] += eg * 3 / 16;
					img[((j + 1) * w + i - 1) * 3 + 2] += eb * 3 / 16;
					img[((j + 1) * w + i) * 3 + 0] += er * 5 / 16;
					img[((j + 1) * w + i) * 3 + 1] += eg * 5 / 16;
					img[((j + 1) * w + i) * 3 + 2] += eb * 5 / 16;
					img[((j + 1) * w + i + 1) * 3 + 0] += er / 16;
					img[((j + 1) * w + i + 1) * 3 + 1] += eg / 16;
					img[((j + 1) * w + i + 1) * 3 + 2] += eb / 16;
				}
			}
		}
	}
	return o;
}