/* C:\Users\dead5\OneDrive\"바탕 화면"\"개발 도구"\Visual */
#include<iostream> 
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<ctime>  
#include<conio.h>
#include<windows.h>
#define spade 4
#define diamond 3
#define heart 2
#define clover 1
#define A 1
#define J 11
#define Q 12
#define K 13

using namespace std;

enum card_rank {
	top = 1, one_pair, two_pair, triple, straight, back_straight, flush, full_house, four_card,
	straight_flush, back_straight_flush, royal_straight_flush
};

class card {
private:
	int number;
	int shape;
public:
	void reset_card() {
		number = rand() % 13 + 1;
		shape = rand() % 4 + 1;
	}	/*	reset card members to random	*/

	void reset_card(card s) {
		number = s.return_number();
		shape = s.return_shape();
	}	/*	reset card members to card class for player card!	*/

	void reset_card(card* s) {
		number = s->return_number();
		shape = s->return_shape();
	}	/*	reset card members to card class for player card!	*/

	void erase_card(int n = 0) {
		number = n;
		shape = 0;
	}	/*	erase card for player's card reset & draw card	*/

	bool cardcmp(card cmp) {
		bool tf = false;
		if (cmp.number == number and cmp.shape == shape)
			tf = true;
		return tf;
	}	/* card compare to avoid duplication	*/

	int return_number() { return number; }	/* return number!	*/
	int return_shape() { return shape; }	/* return shape! */
};

class deck {
private:
	card dck[52];
	int cnt;
public:
	deck() {
		cnt = 0;	/*draw card counter reset!	*/
		int ct = 1;		/* reset card counter */
		dck[0].reset_card();
		srand(time(NULL));
		while (true) {
			if (ct == 52)
				break;

			bool tf = false;
			dck[ct].reset_card();

			for (int i = 0; i < ct; i++) {
				if (dck[ct].cardcmp(dck[i]))
					tf = true;
			}
			if (!tf)
				ct++;
		}
	}	/*	reset deck card(dck) not duplicated!	*/

	card return_card() {
		card cpy;
		cpy.reset_card(dck[cnt]);
		dck[cnt].erase_card();	/* erase dck[cnt] for throw error!	*/
		cnt++;	/* and cnt++ for next card!	*/
		return cpy;
	}	/* function for draw card to player!	*/

};

class deck_sort {
public:
	void srtdeck(card list[]) {
		for (int i = 0; i < 4; i++)
			for (int k = i; k < 5; k++)
				if (list[i].return_number() > list[k].return_number()) {
					card tmp;
					tmp.reset_card(list[i]);
					list[i].reset_card(list[k]);
					list[k].reset_card(tmp);
				}
	}
};
deck_sort sort;

class hand_rank {
public:
	card high;
	int rank;

public:
	void top_check(card list[]) {
		high.erase_card();
		for (int i = 0; i < 5; i++) {
			if (high.return_number() < list[i].return_number())
				high.reset_card(list[i]);
		}

		for (int i = 0; i < 5; i++) {
			if (high.return_number() == list[i].return_number() and high.return_shape() < list[i].return_shape())
				high.reset_card(list[i]);
		}
		rank = 1;
	}	/*	high card check and top check	*/

	void pair_check(card list[]) {
		int pair[14] = { 0, };

		bool one_pair = false;	//2
		bool two_pair = false;	//3
		bool triple = false;	//4
		bool full_house = false;	//9
		bool four_card = false;	//10
		int two = 0;
		for (int i = 0; i < 5; i++)
			pair[list[i].return_number()]++;
		int sum = 0;
		for (int i = 1; i < 14; i++) {
			if (pair[i] == 2) {
				one_pair = true;
				two++;
			}
			if (pair[i] == 3)
				triple = true;
			if (pair[i] == 4)
				four_card = true;
			if (two == 2)
				two_pair = true;
			if (one_pair and triple)
				full_house = true;
			sum += pair[i];
			if (sum == 5)
				break;
		}
		if (one_pair)
			rank = 2;
		if (two_pair)
			rank = 3;
		if (triple)
			rank = 4;
		if (full_house)
			rank = 9;
		if (four_card)
			rank = 10;
	}	/* pair, triple, full house and four card check	*/

	void straight_check(card list[]) {
		bool straight = false;	//	5
		bool bstraight = false;	//	6
		bool flush = false;	//	8
		bool straightf = false; // 11
		bool bstraightf = false;	// 12
		int number[14] = { 0, };
		for (int i = 0; i < 5; i++)
			number[list[i].return_number()]++;
		for (int i = 1; i < 9; i++)
			if (number[i] == 1)
				for (int k = i; k < i + 5; k++) {
					if (number[k] != 1)
						break;
					if (k == i + 4) {
						straight = true;
						rank = 5;
					}
				}
		if (list[0].return_number() == A and straight) {
			bstraight = true;
			rank = 6;
		}

		int shape[5] = { 0, };
		for (int i = 0; i < 5; i++)
			shape[list[i].return_shape()]++;

		for (int i = clover; i < spade + 1; i++)
			if (shape[i] == 5) {
				flush = true;
				rank = 8;
			}

		if (straight and flush) {
			straightf = true;
			rank = 11;
		}

		if (bstraight and flush) {
			bstraightf = true;
			rank = 12;
		}
	}	/* straight, back straight, flush, straight flush and back straight flush check	*/

	void mountain_check(card list[]) {
		bool mountain = false;
		bool flush = false;
		bool rstraightf = false;
		if (list[0].return_number() == A and list[1].return_number() == 10 and list[2].return_number() == J and list[3].return_number() == Q and list[4].return_number() == K) {
			mountain = true;
			rank = 7;
		}
		int shape[5] = { 0, };
		for (int i = 0; i < 5; i++)
			shape[list[i].return_shape()]++;
		for (int i = 0; i < 5; i++)
			if (shape[i] == 5) {
				flush = true;
			}
		if (mountain and flush) {
			rstraightf = true;
			rank = 13;
		}
	}	/*	mountain and royal straight flush check	*/

	void print() {
		switch (rank) {
		case 1:
			cout << "TOP \n";
			break;
		case 2:
			cout << "ONE PAIR \n";
			break;
		case 3:
			cout << "TWO PAIR \n";
			break;
		case 4:
			cout << "TRIPLE \n";
			break;
		case 5:
			cout << "STRAIHGT \n";
			break;
		case 6:
			cout << "BACK STRAIGHT \n";
			break;
		case 7:
			cout << "MOUNTAIN \n";
			break;
		case 8:
			cout << "FLUSH \n";
			break;
		case 9:
			cout << "FULL HOUSE \n";
			break;
		case 10:
			cout << "FOUR CARD \n";
			break;
		case 11:
			cout << "STRAIGHT FLUSH \n";
			break;
		case 12:
			cout << "BACK STRAIGHT FLUSH \n";
			break;
		case 13:
			cout << "ROYAL STRAIGHT FLUSH \n";
			break;
		}
		cout << "HIGH CARD: " << high.return_number() << " ";
		switch (high.return_shape()) {
		case clover:
			cout << "♣\n";
			break;
		case heart:
			cout << "♥\n";
			break;
		case diamond:
			cout << "?\n";
			break;
		case spade:
			cout << "♠\n";
			break;
		}
	}

	int return_rank() { return rank; }
	card return_high() { return high; }

};


class point {
private:
	int xpos;
	int ypos;
public:
	void reset_pos() {
		xpos = rand() % 16 + 1;
		ypos = rand() % 16 + 1;
	}

	void move(char dir) {
		if (dir == 'W' or dir == 'w')
			ypos--;
		if (dir == 'S' or dir == 's')
			ypos++;
		if (dir == 'A' or dir == 'a')
			xpos--;
		if (dir == 'D' or dir == 'd')
			xpos++;

		if (ypos < 0)
			ypos++;
		if (ypos > 15)
			ypos--;

		if (xpos < 0)
			xpos++;
		if (xpos > 15)
			xpos--;
	}

	int return_xpos() { return xpos; }
	int return_ypos() { return ypos; }
};

class unit : public point {
private:
	char shape;
	int hp;	// heart
	int atk;	//	spade
	int amr;	//	diamond
	int luck;	//	clover
public:
	void reset_unit(card list[], hand_rank rank) {
		/*	character stat decided by hand rankings */
		switch (rank.return_rank()) {
			/* TOP */
		case 1:
			hp = 100;
			atk = 10;
			amr = 5;
			luck = 10;
			break;
			/* ONE PAIR */
		case 2:
			hp = 120;
			atk = 13;
			amr = 6;
			luck = 13;
			break;
			/* TWO PAIR */
		case 3:
			hp = 150;
			atk = 17;
			amr = 8;
			luck = 17;
			break;
			/* TRIPLE */
		case 4:
			hp = 190;
			atk = 22;
			amr = 11;
			luck = 22;
			break;
			/* STRAIGHT */
		case 5:
			hp = 240;
			atk = 28;
			amr = 15;
			luck = 28;
			break;
			/* BACK STRAIGHT */
		case 6:
			hp = 300;	// 60
			atk = 35;	// 7
			amr = 20;	// 5
			luck = 35;	// 7
			break;
			/* MOUNTAIN */
		case 7:
			hp = 370;	// 70
			atk = 42;	// 8
			amr = 25;	// 6
			luck = 42;	// 8
			break;
			/* FLUSH */
		case 8:
			hp = 450;	// 80
			atk = 51;	// 9	
			amr = 32;	// 7
			luck = 51;	// 9
			break;
			/* FULL HOUSE */
		case 9:
			hp = 540;	// 90
			atk = 61;	// 10
			amr = 40;	// 8
			luck = 61;	// 10
			break;
			/* FOUR CARD */
		case 10:
			hp = 640;	// 100
			atk = 72;	// 11
			amr = 49;	// 9
			luck = 72;	// 11
			break;
			/* STRAIGHT FLUSH */
		case 11:
			hp = 750;
			atk = 40;
			amr = 25;
			luck = 40;
			break;
			/* BACK STRAIGHT FLUSH */
		case 12:
			hp = 870;
			atk = 53;
			amr = 36;
			luck = 53;
			break;
			/* ROYAL STRAIGHT FLUSH */
		case 13:
			hp = 1000;
			atk = 67;
			amr = 49;
			luck = 67;
			break;
		}

		/*	character stat decided by card hand	*/
		for (int i = 0; i < 5; i++) {
			int plus = list[i].return_number();
			if (plus == A)
				plus = 14;
			switch (list[i].return_shape()) {
			case heart:
				hp += plus;
				break;
			case spade:
				atk += plus;
				break;
			case diamond:
				amr += plus;
				break;
			case clover:
				luck += plus;
				break;
			}
		}
		/* character stat decided by high card	*/
		switch (rank.return_high().return_shape()) {
			int plus = rank.return_high().return_number();
			if (plus == A)
				plus = 14;
			switch (rank.return_high().return_shape()) {
			case heart:
				hp += plus;
				break;
			case spade:
				atk += plus;
				break;
			case diamond:
				amr += plus;
				break;
			case clover:
				luck += plus;
				break;
			}
		}
	}

	void print() {
		cout << "HP: " << hp << "\n";
		cout << "ATK: " << atk << " " << "AMR: " << amr << "\n";
		cout << "LCUK: " << luck << "\n";
	}

	void acess_hp(int n) {
		hp -= n;
	}
	int return_hp() { return hp; }
	int return_atk() { return atk; }
	int return_amr() { return amr; }
	int return_luck() { return luck; }
};





class Map {
private:
	char map[16][16];
public:
	void reset_map() {
		for (int i = 0; i < 16; i++)
			for (int k = 0; k < 16; k++)
				map[i][k] = '.';
	}
	void reset_map(int x, int y, char s) {
		map[y][x] = s;
	}
	void print() {
		for (int i = 0; i < 16; i++) {
			for (int k = 0; k < 16; k++)
				cout << map[i][k];
			cout << "\n";
		}
	}
};



class player {
private:
	card card_hand[5];
	hand_rank rank;
	unit pch;
	int cnt;
public:
	void reset_player() {
		cnt = 0;
		for (int i = 0; i < 5; i++)
			card_hand[i].erase_card();
	}
	void draw(card hand) {
		card_hand[cnt].reset_card(hand);
		cnt++;
	}
	void reset_rank_and_unit() {
		sort.srtdeck(card_hand);

		rank.top_check(card_hand);
		rank.pair_check(card_hand);
		rank.straight_check(card_hand);
		rank.mountain_check(card_hand);

		pch.reset_unit(card_hand, rank);
		pch.reset_pos();
	}
	void print() {
		for (int i = 0; i < 5; i++) {
			cout << card_hand[i].return_number() << " ";
			switch (card_hand[i].return_shape()) {
			case clover:
				cout << "♣\n";
				break;
			case diamond:
				cout << "?\n";
				break;
			case spade:
				cout << "♠\n";
				break;
			case heart:
				cout << "♥\n";
				break;
			}
		}
		rank.print();
		pch.print();
	}

	void move() {
		char dir;
		dir = _getch();
		pch.move(dir);
	}

	void move(unit s) {
		int xs = pch.return_xpos() - s.return_xpos();
		int ys = pch.return_ypos() - s.return_ypos();

		if (xs < 0)
			pch.move('d');
		else if (xs > 0)
			pch.move('a');
		else if (ys < 0)
			pch.move('s');
		else if (ys > 0)
			pch.move('w');
	}
	unit& return_unit() { return pch; }
};


class battle {
private:
	Map map;
public:
	void print(unit player1, unit player2) {
		map.reset_map();
		map.reset_map(player1.return_xpos(), player1.return_ypos(), '@');
		map.reset_map(player2.return_xpos(), player2.return_ypos(), 'D');

		map.print();
	}

	void fight(unit& player1, unit& player2) {
		int xs = player1.return_xpos() - player2.return_xpos();
		int ys = player1.return_ypos() - player2.return_ypos();
		int perc1 = rand() % 100 + 25 + player1.return_luck();
		int perc2 = rand() % 100 + 25 + player2.return_luck();
		if (-1 <= xs and xs <= 1 and -1 <= ys and ys <= 1) {
			int att1 = player1.return_atk() - player2.return_amr();
			int	att2 = player2.return_atk() - player1.return_amr();
			if (att1 <= 0)
				att1 = 1;
			if (att2 <= 0)
				att2 = 1;
			if (perc1 <= 25 + player1.return_luck())
				att1 *= 2;
			if (perc2 <= 25 + player2.return_luck())
				att2 *= 2;
			player1.acess_hp(att2);
			player2.acess_hp(att1);
		}
	}

};
battle feild;

class System {
public:
	void reset_all(player& one, player& two) {
		deck dck;
		one.reset_player();
		two.reset_player();
		for (int i = 1; i < 11; i++) {
			if (i % 2 == 0)
				one.draw(dck.return_card());
			else
				two.draw(dck.return_card());
		}

		one.reset_rank_and_unit();
		two.reset_rank_and_unit();
	}

	void print_all(player one, player two) {
		system("cls");
		feild.print(one.return_unit(), two.return_unit());
		cout << "----player 1---- \n";
		one.print();
		cout << "----player 2---- \n";
		two.print();
	}
};
System sys;

void start() {
	player one;
	player two;
	sys.reset_all(one, two);
	while (true) {
		sys.print_all(one, two);
		if (two.return_unit().return_hp() <= 0) {
			cout << "YOU WIN \n";
			break;
		}
		if (one.return_unit().return_hp() <= 0) {
			cout << "YOU LOSE \n";
			break;
		}
		one.move();
		two.move(one.return_unit());
		feild.fight(one.return_unit(), two.return_unit());
	}
}

int main(void) {
	start();
	return 0;
}







