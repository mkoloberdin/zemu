	unsigned long clk, clMod;
	int *scr, line, pos, zxLine, zxScreen, cl;

	if (nextClk < SCREEN_START) { return; }
	if (prevRenderClk >= SCREEN_END) { return; }

	if (prevRenderClk < SCREEN_START) { prevRenderClk = SCREEN_START; }
	if (nextClk > SCREEN_END) { nextClk = SCREEN_END; }

	for (clk = prevRenderClk; clk < nextClk;)
	{
		clMod = (clk - SCREEN_START) % SCREEN_LINE_TACTS;

		if (clMod < (SCREEN_HOR_OFFSET_TACTS + SCREEN_HRET_TACTS))
		{
			clk += (SCREEN_HOR_OFFSET_TACTS + SCREEN_HRET_TACTS) - clMod;
			continue;
		}

		clMod -= (SCREEN_HOR_OFFSET_TACTS + SCREEN_HRET_TACTS);

		if (clMod >= (WIDTH / SCREEN_TACTS_PER_PIXEL))
		{
			clk += SCREEN_LINE_TACTS - clMod;
			continue;
		}

		line = (clk - SCREEN_START) / SCREEN_LINE_TACTS;

		if ((line < (64 - SCREEN_VERT_OFFSET_LINES)) ||
			(line >= (256 - SCREEN_VERT_OFFSET_LINES)) ||
			(clMod < (36 - SCREEN_HOR_OFFSET_TACTS)) ||
			(clMod >= (164 - SCREEN_HOR_OFFSET_TACTS))
		) {
			cl = colors[dev_border.portFB & 7];
			scr = (int *)renderSurf->pixels + (renderPitch * line) + (clMod * SCREEN_TACTS_PER_PIXEL);
			*(scr++) = cl;
			*(scr) = cl;
			clk++;
		}
		else
		{
			pos = (clMod - (36 - SCREEN_HOR_OFFSET_TACTS)) / 4;
			zxLine = line - (64 - SCREEN_VERT_OFFSET_LINES);
			scr = (int *)renderSurf->pixels + (renderPitch * line) + ((36 - SCREEN_HOR_OFFSET_TACTS) * SCREEN_TACTS_PER_PIXEL) + (pos * SCREEN_TACTS_PER_PIXEL * 4);
