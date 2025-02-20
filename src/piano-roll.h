#ifndef PIANO_ROLL_H
#define PIANO_ROLL_H

#include <array>
#include <set>
#include <vector>

#pragma warning(push, 0)
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#pragma warning(pop)

#include "command.h"
#include "song.h"
#include "widgets.h"

const Fl_Color BACKGROUND3_COLOR = FL_FREE_COLOR;

const Fl_Color NOTE_RED   = fl_rgb_color(217,   0,   0);
const Fl_Color NOTE_BLUE  = fl_rgb_color(  0, 117, 253);
const Fl_Color NOTE_GREEN = fl_rgb_color(  3, 180,   3);
const Fl_Color NOTE_BROWN = fl_rgb_color(140,  60,  25);

const Fl_Color NOTE_RED_LIGHT   = fl_lighter(NOTE_RED);
const Fl_Color NOTE_BLUE_LIGHT  = fl_lighter(NOTE_BLUE);
const Fl_Color NOTE_GREEN_LIGHT = fl_lighter(NOTE_GREEN);
const Fl_Color NOTE_BROWN_LIGHT = fl_lighter(NOTE_BROWN);

const Fl_Color NOTE_GHOST = fl_rgb_color(96);

const Fl_Color FLAG_SPEED     = FL_CYAN;
const Fl_Color FLAG_VOLUME    = FL_MAGENTA;
const Fl_Color FLAG_VIBRATO   = FL_YELLOW;
const Fl_Color FLAG_DUTY_WAVE = fl_rgb_color(255, 128, 0);

constexpr size_t NUM_WHITE_NOTES = 7;
constexpr size_t NUM_BLACK_NOTES = 5;

constexpr size_t NUM_NOTES_PER_OCTAVE = NUM_WHITE_NOTES + NUM_BLACK_NOTES;
constexpr size_t NUM_OCTAVES = 8;

constexpr int WHITE_KEY_WIDTH  = 150;
constexpr int WHITE_KEY_HEIGHT_ZOOMED = 36;
constexpr int WHITE_KEY_HEIGHT_UNZOOMED = 24;

constexpr int BLACK_KEY_WIDTH  = 100;
constexpr int BLACK_KEY_HEIGHT_ZOOMED = 30;
constexpr int BLACK_KEY_HEIGHT_UNZOOMED = 20;

constexpr int TICK_WIDTH_ZOOMED = 4;
constexpr int TICK_WIDTH_UNZOOMED = 3;
constexpr int TICKS_PER_STEP = 12;

struct Note_Key {
	int y, delta1, delta2;
	const char *label;
	Pitch pitch;
	bool white;
};

constexpr Note_Key NOTE_KEYS[NUM_NOTES_PER_OCTAVE] {
	{  0,  0,  0, "B",     Pitch::B_NAT,   true },
	{  1, +1,  0, "A",     Pitch::A_NAT,   true },
	{  2, +1, +1, "G",     Pitch::G_NAT,   true },
	{  3, +1, +1, "F",     Pitch::F_NAT,   true },
	{  4, -1, -1, "E",     Pitch::E_NAT,   true },
	{  5, -1, -1, "D",     Pitch::D_NAT,   true },
	{  6, -1,  0, "C",     Pitch::C_NAT,   true },
	{  1,  0,  0, "B♭/A♯", Pitch::A_SHARP, false },
	{  3,  0,  0, "A♭/G♯", Pitch::G_SHARP, false },
	{  5,  0,  0, "G♭/F♯", Pitch::F_SHARP, false },
	{  8,  0,  0, "E♭/D♯", Pitch::D_SHARP, false },
	{ 10,  0,  0, "D♭/C♯", Pitch::C_SHARP, false },
};
constexpr size_t PITCH_TO_KEY_INDEX[NUM_NOTES_PER_OCTAVE] {
	6,  // C
	11, // C#
	5,  // D
	10, // D#
	4,  // E
	3,  // F
	9,  // F#
	2,  // G
	8,  // G#
	1,  // A
	7,  // A#
	0,  // B
};

constexpr char const *C_Key_Labels[] = {
	"C1",
	"C2",
	"C3",
	"C4",
	"C5",
	"C6",
	"C7",
	"C8",
};

class Note_Box : public Fl_Box {
private:
	const Note_View &_note_view;
	int32_t _tick = 0;
	bool _selected = false;
public:
	Note_Box(const Note_View &n, int32_t t, int X, int Y, int W, int H, const char *l = nullptr)
		: Fl_Box(X, Y, W, H, l), _note_view(n), _tick(t) {}

	inline const Note_View &note_view(void) const { return _note_view; }
	inline int32_t tick(void) const { return _tick; }
	inline bool selected(void) const { return _selected; }
	inline void selected(bool s) { _selected = s; }

	inline bool ghost(void) const { return _note_view.ghost; }
protected:
	void draw() override;
};

class Wrapper_Box : public Fl_Box {
private:
	int32_t _start_tick = 0;
	int32_t _end_tick = 0;
	Pitch _min_pitch = Pitch::B_NAT;
	int32_t _min_octave = 8;
	Pitch _max_pitch = Pitch::C_NAT;
	int32_t _max_octave = 1;
public:
	using Fl_Box::Fl_Box;

	inline int32_t start_tick(void) const { return _start_tick; }
	inline int32_t end_tick(void) const { return _end_tick; }
	inline Pitch min_pitch(void) const { return _min_pitch; }
	inline int32_t min_octave(void) const { return _min_octave; }
	inline Pitch max_pitch(void) const { return _max_pitch; }
	inline int32_t max_octave(void) const { return _max_octave; }

	inline void set_start_tick(int32_t t) { _start_tick = t; }
	inline void set_end_tick(int32_t t) { _end_tick = t; }
	inline void set_min_pitch(Pitch p, int32_t o) { _min_pitch = p; _min_octave = o; }
	inline void set_max_pitch(Pitch p, int32_t o) { _max_pitch = p; _max_octave = o; }
};

class Loop_Box : public Wrapper_Box {
public:
	using Wrapper_Box::Wrapper_Box;
protected:
	void draw() override;
};

class Call_Box : public Wrapper_Box {
public:
	using Wrapper_Box::Wrapper_Box;
protected:
	void draw() override;
};

class Flag_Box : public Fl_Box {
	int32_t _note_index = 0;
	int32_t _row_offset = 0;
public:
	Flag_Box(int32_t i, int32_t o, int X, int Y, int W, int H, const char *l = nullptr)
		: Fl_Box(X, Y, W, H, l), _note_index(i), _row_offset(o) {}

	inline int32_t note_index(void) const { return _note_index; }
	inline int32_t row_offset(void) const { return _row_offset; }
};

class Piano_Keys;

class Key_Box : public Fl_Box {
private:
	Pitch _pitch;
	int32_t _octave;
public:
	Key_Box(Pitch p, int32_t o, int X, int Y, int W, int H, const char *l = nullptr)
		: Fl_Box(X, Y, W, H, l), _pitch(p), _octave(o) {}

	Piano_Keys *parent() const { return (Piano_Keys *)Fl_Box::parent(); }

	inline Pitch pitch(void) const { return _pitch; }
	inline int32_t octave(void) const { return _octave; }

	int handle(int event) override;
protected:
	void draw() override;
};

class White_Key_Box : public Key_Box {
public:
	using Key_Box::Key_Box;
protected:
	void draw() override;
};

class Piano_Timeline;

class Piano_Keys : public Fl_Group {
private:
	std::array<Key_Box *, NUM_NOTES_PER_OCTAVE * NUM_OCTAVES> _keys;

	Pitch   _channel_1_pitch = Pitch::REST;
	int32_t _channel_1_octave = 0;
	Pitch   _channel_2_pitch = Pitch::REST;
	int32_t _channel_2_octave = 0;
	Pitch   _channel_3_pitch = Pitch::REST;
	int32_t _channel_3_octave = 0;
	Pitch   _channel_4_pitch = Pitch::REST;
	int32_t _channel_4_octave = 0;
public:
	Piano_Keys(int X, int Y, int W, int H, const char *l = nullptr);

	Piano_Keys(const Piano_Keys&) = delete;
	Piano_Keys& operator=(const Piano_Keys&) = delete;

	Piano_Timeline *parent() const { return (Piano_Timeline *)Fl_Group::parent(); }

	void calc_sizes();
	void key_labels(bool show);

	void set_key_color(Pitch pitch, int32_t octave, Fl_Color color);
	void update_key_colors();
	void reset_key_colors();

	bool find_key_below_mouse(Key_Box *&key);

	inline void set_channel_1_pitch(Pitch p, int32_t o) { _channel_1_pitch = p; _channel_1_octave = o; }
	inline void set_channel_2_pitch(Pitch p, int32_t o) { _channel_2_pitch = p; _channel_2_octave = o; }
	inline void set_channel_3_pitch(Pitch p, int32_t o) { _channel_3_pitch = p; _channel_3_octave = o; }
	inline void set_channel_4_pitch(Pitch p, int32_t o) { _channel_4_pitch = p; _channel_4_octave = o; }

	void set_channel_pitch(int channel_number, Pitch p, int32_t o);
	void reset_channel_pitches();
};

class Piano_Roll;

class Piano_Timeline : public Fl_Group {
	friend class Piano_Roll;
private:
	Fl_Group _trashcan;
	Fl_End _end;
	Piano_Keys _keys;
	std::vector<Note_Box *> _channel_1_notes;
	std::vector<Note_Box *> _channel_2_notes;
	std::vector<Note_Box *> _channel_3_notes;
	std::vector<Note_Box *> _channel_4_notes;
	std::vector<Loop_Box *> _channel_1_loops;
	std::vector<Loop_Box *> _channel_2_loops;
	std::vector<Loop_Box *> _channel_3_loops;
	std::vector<Loop_Box *> _channel_4_loops;
	std::vector<Call_Box *> _channel_1_calls;
	std::vector<Call_Box *> _channel_2_calls;
	std::vector<Call_Box *> _channel_3_calls;
	std::vector<Call_Box *> _channel_4_calls;
	std::vector<Flag_Box *> _channel_1_flags;
	std::vector<Flag_Box *> _channel_2_flags;
	std::vector<Flag_Box *> _channel_3_flags;
	std::vector<Flag_Box *> _channel_4_flags;

	int32_t _cursor_tick = -1;
public:
	Piano_Timeline(int X, int Y, int W, int H, const char *l = nullptr);
	~Piano_Timeline() noexcept;

	void clear();
	void clear_channel_1();
	void clear_channel_2();
	void clear_channel_3();
	void clear_channel_4();

	Piano_Timeline(const Piano_Timeline&) = delete;
	Piano_Timeline& operator=(const Piano_Timeline&) = delete;

	Piano_Roll *parent() const { return (Piano_Roll *)Fl_Group::parent(); }
	inline int selected_channel() const;

	void calc_sizes();

	int handle(int event) override;
	bool handle_note_pencil(int event);
	bool handle_note_eraser(int event);
	bool handle_note_selection(int event);
	bool select_all();
	bool select_none();

	void highlight_channel_1_tick(int32_t tick, bool muted) { highlight_tick(_channel_1_notes, 1, tick, muted, NOTE_RED_LIGHT); }
	void highlight_channel_2_tick(int32_t tick, bool muted) { highlight_tick(_channel_2_notes, 2, tick, muted, NOTE_BLUE_LIGHT); }
	void highlight_channel_3_tick(int32_t tick, bool muted) { highlight_tick(_channel_3_notes, 3, tick, muted, NOTE_GREEN_LIGHT); }
	void highlight_channel_4_tick(int32_t tick, bool muted) { highlight_tick(_channel_4_notes, 4, tick, muted, NOTE_BROWN_LIGHT); }

	void set_channel_1(const std::vector<Note_View> &notes) { set_channel(_channel_1_notes, _channel_1_flags, 1, notes, NOTE_RED); }
	void set_channel_2(const std::vector<Note_View> &notes) { set_channel(_channel_2_notes, _channel_2_flags, 2, notes, NOTE_BLUE); }
	void set_channel_3(const std::vector<Note_View> &notes) { set_channel(_channel_3_notes, _channel_3_flags, 3, notes, NOTE_GREEN); }
	void set_channel_4(const std::vector<Note_View> &notes) { set_channel(_channel_4_notes, _channel_4_flags, 4, notes, NOTE_BROWN); }

	void set_channel_1_detail(int detail) { set_channel_detail(_channel_1_notes, _channel_1_loops, _channel_1_calls, _channel_1_flags, detail); }
	void set_channel_2_detail(int detail) { set_channel_detail(_channel_2_notes, _channel_2_loops, _channel_2_calls, _channel_2_flags, detail); }
	void set_channel_3_detail(int detail) { set_channel_detail(_channel_3_notes, _channel_3_loops, _channel_3_calls, _channel_3_flags, detail); }
	void set_channel_4_detail(int detail) { set_channel_detail(_channel_4_notes, _channel_4_loops, _channel_4_calls, _channel_4_flags, detail); }

	void reset_note_colors();
private:
	void highlight_tick(std::vector<Note_Box *> &notes, int channel_number, int32_t tick, bool muted, Fl_Color color);
	void select_note_at_tick(std::vector<Note_Box *> &notes, int32_t tick);
	void set_channel(std::vector<Note_Box *> &channel, std::vector<Flag_Box *> &flags, int channel_number, const std::vector<Note_View> &notes, Fl_Color color);
	void set_channel_detail(std::vector<Note_Box *> &notes, std::vector<Loop_Box *> &loops, std::vector<Call_Box *> &calls, std::vector<Flag_Box *> &flags, int detail);

	std::vector<Note_Box *> *active_channel_boxes();
protected:
	void draw() override;
};

class Main_Window;

class Piano_Roll : public OS_Scroll {
private:
	int32_t _tick = -1;
	bool _following = false;
	bool _continuous = true;
	bool _paused = false;
	bool _zoomed = false;
	int _ticks_per_step = TICKS_PER_STEP;

	bool _channel_1_muted = false;
	bool _channel_2_muted = false;
	bool _channel_3_muted = false;
	bool _channel_4_muted = false;

	Piano_Timeline _piano_timeline;

	std::vector<Note_View> _channel_1_notes;
	std::vector<Note_View> _channel_2_notes;
	std::vector<Note_View> _channel_3_notes;
	std::vector<Note_View> _channel_4_notes;
	int32_t _channel_1_loop_tick = -1;
	int32_t _channel_2_loop_tick = -1;
	int32_t _channel_3_loop_tick = -1;
	int32_t _channel_4_loop_tick = -1;
	int32_t _channel_1_end_tick = -1;
	int32_t _channel_2_end_tick = -1;
	int32_t _channel_3_end_tick = -1;
	int32_t _channel_4_end_tick = -1;

	int32_t _song_length = -1;

	std::vector<Fl_Widget *> _correlates;
public:
	Piano_Roll(int X, int Y, int W, int H, const char *l = nullptr);
	~Piano_Roll() noexcept;

	Piano_Roll(const Piano_Roll&) = delete;
	Piano_Roll& operator=(const Piano_Roll&) = delete;

	inline void add_correlate(Fl_Widget *wgt) { _correlates.push_back(wgt); }
	inline void clear_correlates(void) { _correlates.clear(); }

	Main_Window *parent() const { return (Main_Window *)OS_Scroll::parent(); }
	inline int selected_channel() const;

	inline int32_t tick(void) const { return _tick; }
	inline bool following(void) const { return _following; }
	inline bool paused(void) const { return _paused; }
	inline bool zoomed(void) const { return _zoomed; }
	inline int ticks_per_step(void) const { return _ticks_per_step; }

	void tick(int32_t t) { _tick = t; }

	void channel_1_muted(bool m) { _channel_1_muted = m; }
	void channel_2_muted(bool m) { _channel_2_muted = m; }
	void channel_3_muted(bool m) { _channel_3_muted = m; }
	void channel_4_muted(bool m) { _channel_4_muted = m; }

	int white_key_height() const;
	int black_key_height() const;
	int octave_height() const;
	int note_row_height() const;
	int black_key_offset() const;
	int tick_width() const;

	inline const std::vector<Note_View> &channel_1_notes() const { return _channel_1_notes; }
	inline const std::vector<Note_View> &channel_2_notes() const { return _channel_2_notes; }
	inline const std::vector<Note_View> &channel_3_notes() const { return _channel_3_notes; }
	inline const std::vector<Note_View> &channel_4_notes() const { return _channel_4_notes; }
	inline int32_t channel_1_loop_tick() const { return _channel_1_loop_tick; }
	inline int32_t channel_2_loop_tick() const { return _channel_2_loop_tick; }
	inline int32_t channel_3_loop_tick() const { return _channel_3_loop_tick; }
	inline int32_t channel_4_loop_tick() const { return _channel_4_loop_tick; }
	inline int32_t channel_1_end_tick() const { return _channel_1_end_tick; }
	inline int32_t channel_2_end_tick() const { return _channel_2_end_tick; }
	inline int32_t channel_3_end_tick() const { return _channel_3_end_tick; }
	inline int32_t channel_4_end_tick() const { return _channel_4_end_tick; }
	inline int32_t song_length(void) const { return _song_length; }

	int handle(int event) override;
	bool handle_mouse_click(int event);

	void refresh_note_properties();

	void set_continuous_scroll(bool c) { _continuous = c; }

	void step_backward();
	void step_forward();
	void skip_backward();
	void skip_forward();
	void zoom(bool z);
	void ticks_per_step(int t) { _ticks_per_step = t; }
	void key_labels(bool show) { _piano_timeline._keys.key_labels(show); }

	void set_size(int W, int H);
	void set_timeline_width();

	void set_timeline(const Song &song);
	void set_active_channel_timeline(const Song &song);
	void set_active_channel_selection(const std::set<int32_t> &selection);
	void select_note_at_tick();

	void build_note_view(std::vector<Loop_Box *> &loops, std::vector<Call_Box *> &calls, std::vector<Note_View> &notes, const std::vector<Command> &commands, int32_t end_tick, Fl_Color color);

	int32_t get_song_length() const;
	int32_t get_loop_tick() const;
	int32_t get_last_note_x() const;

	void update_channel_detail(int channel_number);

	void align_cursor();

	void clear();

	void start_following();
	void unpause_following();
	void stop_following();
	void pause_following();
	void highlight_tick(int32_t t);
	void focus_cursor(bool center = false);
	void sticky_keys();

	void scroll_to_y_max();
	void scroll_to(int X, int Y);

	int scroll_x_max() const;
	int scroll_y_max() const;

	bool put_note(Song &song, Pitch pitch, int32_t octave, int32_t tick);
	bool set_speed(Song &song, int32_t speed);
	bool set_volume(Song &song, int32_t volume);
	bool set_fade(Song &song, int32_t fade);
	bool set_vibrato_delay(Song &song, int32_t delay);
	bool set_vibrato_extent(Song &song, int32_t extent);
	bool set_vibrato_rate(Song &song, int32_t rate);
	bool set_wave(Song &song, int32_t wave);
	bool set_duty(Song &song, int32_t duty);
	bool set_tempo(Song &song, int32_t tempo);
	bool set_transpose_octaves(Song &song, int32_t octaves);
	bool set_transpose_pitches(Song &song, int32_t pitches);
	bool set_slide_duration(Song &song, int32_t duration);
	bool set_slide_octave(Song &song, int32_t octave);
	bool set_slide_pitch(Song &song, Pitch pitch);
	bool set_slide(Song &song, int32_t duration, int32_t octave, Pitch pitch);

	bool pitch_up(Song &song, bool dry_run = false);
	bool pitch_down(Song &song, bool dry_run = false);
	bool octave_up(Song &song, bool dry_run = false);
	bool octave_down(Song &song, bool dry_run = false);
	bool move_left(Song &song, bool dry_run = false);
	bool move_right(Song &song, bool dry_run = false);
	bool shorten(Song &song, bool dry_run = false);
	bool lengthen(Song &song, bool dry_run = false);
	bool delete_selection(Song &song, bool dry_run = false);
	bool snip_selection(Song &song, bool dry_run = false);
	bool split_note(Song &song);
	bool glue_note(Song &song);

	bool select_all() { return _piano_timeline.select_all(); }
	bool select_none() { return _piano_timeline.select_none(); }

	int32_t quantize_tick(int32_t tick, bool round = false);
private:
	const Note_View *find_note_view_at_tick(const std::vector<Note_View> &view, int32_t tick, int32_t *tick_offset = nullptr);

	std::vector<Note_View> *active_channel_view();
	int32_t active_channel_length();

	static void scrollbar_cb(Fl_Scrollbar *sb, void *);
	static void hscrollbar_cb(Fl_Scrollbar *sb, void *);
};

#endif
