static double battle_menu_rows(void) {
    if (battle_menu_state == 2) {
        return 2;
    }
    return game_state == ST_SOLO ? 3 : 2;
}

static double battle_menu_note_h(void) {
    if (battle_menu_state == 2 || game_state == ST_ONLINE) {
        return 24;
    }
    return 0;
}

static double battle_menu_button_h(void) {
    double rows = battle_menu_rows();
    double gaps = (rows - 1) * 12;
    double fixed = 20 + 34 + battle_menu_note_h() + 12 + gaps + 20;
    double h = (screen_h - 24 - fixed) / rows;
    if (h > btn_h) {
        h = btn_h;
    }
    if (h < 30) {
        h = 30;
    }
    return h;
}

static double battle_menu_panel_h(void) {
    double rows = battle_menu_rows();
    return 20 + 34 + battle_menu_note_h() + 12 + rows * battle_menu_button_h() + (rows - 1) * 12 + 20;
}

static double battle_menu_panel_w(void) {
    return fit_w(560);
}

static double battle_menu_panel_x(void) {
    return center_x(battle_menu_panel_w());
}

static double battle_menu_panel_y(void) {
    double y = (screen_h - battle_menu_panel_h()) / 2;
    return y < 8 ? 8 : y;
}

static double battle_menu_button_w(void) {
    double w = battle_menu_panel_w() - 36;
    return w > 360 ? 360 : w;
}

static double battle_menu_button_x(void) {
    return center_x(battle_menu_button_w());
}

static double battle_menu_button_y(double row) {
    double top = battle_menu_panel_y() + 20 + 34 + battle_menu_note_h() + 12;
    return top + row * (battle_menu_button_h() + 12);
}

static double battle_menu_hit(double x, double y, double row) {
    double pad_y = touch_pad_y < 6 ? touch_pad_y : 6;
    return hit_rect(x, y, battle_menu_button_x() - touch_pad_x, battle_menu_button_y(row) - pad_y,
                    battle_menu_button_w() + 2 * touch_pad_x, battle_menu_button_h() + 2 * pad_y);
}

static const char *battle_menu_title(void) {
    if (battle_menu_state == 2) {
        return tr_leave_confirm_title();
    }
    if (game_state == ST_ONLINE) {
        return tr_match_menu_title();
    }
    return tr_pause_title();
}

static const char *battle_menu_note(void) {
    if (battle_menu_state == 2) {
        return tr_leave_confirm_note();
    }
    if (game_state == ST_ONLINE) {
        return tr_match_menu_note();
    }
    return "";
}

static const char *battle_menu_button_label(double row) {
    if (battle_menu_state == 2) {
        return row == 0 ? tr_cancel_leave() : tr_leave_match();
    }
    if (row == 0) {
        return tr_resume();
    }
    if (game_state == ST_SOLO && row == 1) {
        return tr_restart_match();
    }
    return game_state == ST_SOLO ? tr_exit_lobby() : tr_leave_match();
}

static void draw_battle_menu_button(void) {
    button_scaled(tr_match_menu(), center_x(btn_w), back_y, btn_w, btn_h, 0.8);
}

static void draw_battle_menu_overlay(void) {
    if (battle_menu_state == 0 || finished != 0) {
        return;
    }
    rect(0, 0, screen_w, screen_h, 0xC0000000);
    double x = battle_menu_panel_x();
    double y = battle_menu_panel_y();
    double w = battle_menu_panel_w();
    double h = battle_menu_panel_h();
    roundrect(x, y, w, h, 24, card_bg);
    const char *title = battle_menu_title();
    double title_scale = fit_text_scale(title, w - 36, 0.82);
    ctext_scaled(title, y + 14, text_color, title_scale);
    const char *note = battle_menu_note();
    if (str_len(note) > 0) {
        ctext_scaled(note, y + 49, text_color, fit_text_scale(note, w - 28, 0.48));
    }
    double row = 0;
    while (row < battle_menu_rows()) {
        const char *label = battle_menu_button_label(row);
        double scale = fit_text_scale(label, battle_menu_button_w() - 20, 0.72);
        button_scaled(label, battle_menu_button_x(), battle_menu_button_y(row), battle_menu_button_w(),
                      battle_menu_button_h(), scale);
        row = row + 1;
    }
}

static void battle_menu_clear_input(void) {
    aim_id = -1;
    super_id = -1;
    drop_joy();
}

static void battle_menu_open(void) {
    if (battle_menu_state != 0 || finished != 0) {
        return;
    }
    battle_menu_state = 1;
    battle_menu_clear_input();
}

static void battle_menu_back(void) {
    if (battle_menu_state == 2) {
        battle_menu_state = 1;
    } else {
        battle_menu_state = 0;
    }
    battle_menu_clear_input();
}

static double battle_simulation_paused(void) {
    return game_state == ST_SOLO && battle_menu_state != 0;
}

static void battle_menu_touch(double x, double y, double action) {
    if (action != 0) {
        return;
    }
    double row = 0;
    while (row < battle_menu_rows()) {
        if (battle_menu_hit(x, y, row) == 1) {
            play_button_click();
            if (battle_menu_state == 2) {
                if (row == 0) {
                    battle_menu_state = 1;
                } else {
                    start_transition(ST_LOBBY);
                }
                return;
            }
            if (row == 0) {
                battle_menu_state = 0;
                return;
            }
            if (game_state == ST_SOLO && row == 1) {
                init_game();
                return;
            }
            battle_menu_state = 2;
            return;
        }
        row = row + 1;
    }
}
