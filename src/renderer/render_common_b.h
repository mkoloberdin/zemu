            //// In render_common_a.h there is:
            //// if (clMod >= (WIDTH / SCREEN_TACTS_PER_PIXEL)) { ... continue; }
            ////
            //// WIDTH = 320; SCREEN_TACTS_PER_PIXEL = 2; WIDTH / SCREEN_TACTS_PER_PIXEL = 160
            ////
            //// So we know for sure, that clMod <= 160.
            //// Probably this is leftover from older times, but we need to investigate more.
            //
            // if (clMod <= 160) {
                clk += 4;
            // } else {
            //     clk += 160 - clMod;
            // }
        }
    }

    prevRenderClk = clk;
