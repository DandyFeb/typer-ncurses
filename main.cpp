#include "main.h"

void usage(char *argv)
{
	//std::cout << "usage: " << argv << "[options]\n"
	std::cout << "    usage: type [options]\n"
			  << "    -l string_length \n"
			  << "\tset the string length for each word\n"
			  << "    -p passes\n"
			  << "\tset how many words to type\n"
			  << "    -U  include uppercase characters\n"
			  << "    -L  include lowercase characters\n"
			  << "    -N  include number characters\n"
			  << "    -S  include special characters\n"
			  << "    -A  include all sets of characters above\n";
}

void generate_str()
{
	std::uniform_int_distribution<> dis(0, charset.length()-1);
	shown_str = "";

	for (int i = 0; i < str_len; i++)
		shown_str += charset[dis(gen)];
}

void start_typing(WINDOW *win)
{
	start_interval = std::chrono::steady_clock::now();

	curs_set(1);
	for (int a = 0; a < shown_str.length(); a++)
	{
		move(scr_y/2, scr_x/2-shown_str.length()/2+a);
		char i = getch();
		if (i == shown_str[a])
			wattron(win, COLOR_PAIR(2));
		else
			wattron(win, COLOR_PAIR(1));
		mvwaddch(win, scr_y/2, scr_x/2-shown_str.length()/2+a, shown_str[a]);
		wrefresh(win);
		wattroff(win, COLOR_PAIR(2));
		wattroff(win, COLOR_PAIR(1));
	}
	curs_set(0);

	end_interval = std::chrono::steady_clock::now();
}

int main(int argc, char *argv[])
{
	ARGBEGIN
	{
		case 'l':
			str_len = atoi(ARGF());
			break;
		case 'A':
			charset += uppercase;
			charset += lowercase;
			charset += numbers;
			charset += special;
			break;
		case 'U':
			charset += uppercase;
			break;
		case 'L':
			charset += lowercase;
			break;
		case 'N':
			charset += numbers;
			break;
		case 'S':
			charset += special;
			break;
		default:
			usage(argv[0]);
			return 1;
			break;
	}
	ARGEND;

	if (charset == "")
	{
		usage(argv[0]);
		return 1;
	}

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	start_color();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	getmaxyx(stdscr, scr_y, scr_x);
	WINDOW *main = newwin(scr_y, scr_x, 0, 0);
	box(main, 0, 0);

	bool state = true;
	while (state)
	{
		//TODO 
		generate_str();
		mvwprintw(main, scr_y/2, scr_x/2-str_len/2, "%s", shown_str.c_str());

		refresh();
		wrefresh(main);

		start_typing(main);

		mvwprintw(main, scr_y/2+2, scr_x/2-2, "%d%s", std::chrono::duration_cast<std::chrono::milliseconds>(end_interval - start_interval), "ms");

		refresh();
		wrefresh(main);

		getch();
		wclear(main);

		box(main, 0, 0);
	}

	endwin();

	return 0;
}
