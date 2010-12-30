			if (clMod <= 160) clk += 4;
			else clk += 160 - clMod;
		}
	}

	prevRenderClk = clk;
