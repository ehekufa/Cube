static double tutorial_grid_w(void) {
    return fit_w(780);
}

static double tutorial_gap(void) {
    return 14;
}

static double tutorial_card_w(void) {
    return (tutorial_grid_w() - tutorial_gap()) / 2;
}

static double tutorial_card_x(double col) {
    double left = center_x(tutorial_grid_w());
    return left + col * (tutorial_card_w() + tutorial_gap());
}

static double tutorial_top(void) {
    return title_y() + 34;
}

static double tutorial_button_h(void) {
    double h = btn_h;
    double fit = (screen_h - 28) / 7;
    if (fit < h) {
        h = fit;
    }
    if (h < 28) {
        h = 28;
    }
    return h;
}

static double tutorial_button_y(void) {
    return screen_h - tutorial_button_h() - 14;
}

static double tutorial_card_h(void) {
    double h = (tutorial_button_y() - tutorial_top() - 24 - tutorial_gap()) / 2;
    return clamp(h, 44, 104);
}

static double tutorial_card_y(double row) {
    return tutorial_top() + row * (tutorial_card_h() + tutorial_gap());
}

static double tutorial_start_w(void) {
    return fit_w(420);
}

static double hit_tutorial_start(double x, double y) {
    return hit_rect_pad(x, y, center_x(tutorial_start_w()), tutorial_button_y(), tutorial_start_w(),
                        tutorial_button_h());
}

static void draw_tutorial_card(double col, double row, const char *title, const char *body) {
    double x = tutorial_card_x(col);
    double y = tutorial_card_y(row);
    double w = tutorial_card_w();
    double h = tutorial_card_h();
    double pad = 14;
    roundrect(x, y, w, h, 14, card_bg);
    double title_scale = fit_text_scale(title, w - 2 * pad, 0.56);
    double body_scale = fit_text_scale(body, w - 2 * pad, 0.48);
    text_scaled(title, x + pad, y + 10, text_color, title_scale);
    text_scaled(body, x + pad, y + 38, text_color, body_scale);
}

static void draw_tutorial(void) {
    screen_fill();
    draw_back();
    ctext_scaled(tr_how_to_play(), title_y(), text_color, 0.9);
    draw_tutorial_card(0, 0, tr_tutorial_move_title(), tr_tutorial_move_body());
    draw_tutorial_card(1, 0, tr_tutorial_attack_title(), tr_tutorial_attack_body());
    draw_tutorial_card(0, 1, tr_tutorial_ability_title(), tr_tutorial_ability_body());
    draw_tutorial_card(1, 1, tr_tutorial_progress_title(), tr_tutorial_progress_body());
    button_scaled(tr_tutorial_start(), center_x(tutorial_start_w()), tutorial_button_y(), tutorial_start_w(),
                  tutorial_button_h(), 0.76);
}

static void touch_tutorial(double x, double y, double action) {
    if (action != 0) {
        return;
    }
    if (back_hit(x, y) == 1) {
        start_transition(ST_MODES);
    } else if (hit_tutorial_start(x, y) == 1) {
        start_transition(ST_SOLO);
    }
}
