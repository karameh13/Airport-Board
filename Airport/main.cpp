#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

enum Time_Points : unsigned int { ACT , CET , CST , EST , GMT , MSK , PST , UTC };
enum arr_dep { arrival, departure };

class LocalTime
{
private:
	int minutes;
	Time_Points time_zone;
public:
	LocalTime(int Minute = -1, Time_Points Time_Zone = CET)
	{
		minutes = Minute;
		time_zone = Time_Zone;
	}
	LocalTime(int Hour, int Minute, Time_Points Time_Zone = CET)
	{;
		time_zone = Time_Zone;
		set_time(Hour, Minute);
	}
	void set_time(int Hour, int Minute)
	{
	    if (Hour >=0 && Minute >=0 && Hour <24 && Minute <60)
            minutes = Hour * 60 + Minute;
		else
            minutes= -1;
	}
	int get_hour()
	{
		return minutes / 60;
	}
	int get_minute()
	{
		return minutes % 60;
	}
	bool is_valid()
	{
		if (minutes < 0)
			return false;
		else
			return true;
	}
	friend ostream& operator <<(ostream& output, LocalTime& ob);
};

ostream& operator <<(ostream& output, LocalTime& ob)
{
	output << setw(2) << setfill('0') << ob.get_hour()
		<< ":" << setw(2) << setfill('0') << ob.get_minute();
	return output;
}

class Flight
{
private:
	arr_dep arrivalOrDeparture;
	string code, destination, gate, checkIn, comment;
	LocalTime expected;
public:
	Flight(arr_dep exp_arr_dep, string Code, string Destination, string Gate, string CheckIn = "");
	~Flight();
	string get_code();
	string get_destination();
	string get_gate();
	string get_checkIn();
	string get_comment();
	LocalTime get_scheduled();
	void set_expected(LocalTime Expected);
	void set_comment(string Comment);
	bool is_arrival()
	{
	    arrivalOrDeparture = arrival;
		return arrivalOrDeparture;
	}
	bool is_departure()
	{
	    arrivalOrDeparture = departure;
		return arrivalOrDeparture;
	}
	void print_fun(LocalTime x);
};

Flight::Flight(arr_dep exp_arr_dep, string Code,
	string Destination, string Gate, string CheckIn)
{
	arrivalOrDeparture = exp_arr_dep;
	code = Code;
	destination = Destination;
	gate = Gate;
	checkIn = CheckIn;
	comment = "";
}
Flight :: ~Flight()
{
	cout << "flight " << code << " deleted " << endl;
}

string Flight::get_code()
{
	return code;
}
string Flight::get_destination()
{
	return destination;
}
string Flight::get_gate()
{
	return gate;
}
string Flight::get_checkIn()
{
	return checkIn;
}
string Flight::get_comment()
{
	return comment;
}
LocalTime Flight::get_scheduled()
{
	return expected;
}
void Flight::set_expected(LocalTime Expected)
{
	expected = Expected;
}
void Flight::set_comment(string Comment)
{
	comment = Comment;
}
void Flight::print_fun(LocalTime x)
{
	cout << get_code() << get_destination() <<x.get_hour() << ':' << x.get_minute()
         << "\t\t   " << get_gate() << get_checkIn() << get_comment() << endl;
}

class Airport
{
private:
	string name;
	Flight* slot[24][60] = {};
public:
	Airport(string Name)
	{
		name = Name;
	}
	~Airport()
	{
		for (int i = 0; i < 24;i++)
		{
			for (int j = 0;j < 60;j++)
				 {
				     delete slot[i][j];
				 }
		}
	}
	void schedule(LocalTime t, Flight* f)
	{
	    f->set_expected(t);
	    slot[t.get_hour()][t.get_minute()] = f;
	}

	void comment(LocalTime t, string n)
	{
	    slot[t.get_hour()][t.get_minute()]->set_comment(n);
        slot[t.get_hour()][t.get_minute()]->get_comment();
	}

	void delay(LocalTime t1, LocalTime t2)
	{
        slot[t1.get_hour()][t1.get_minute()]->set_expected(t2);
	}

	void print(arr_dep x)
	{
        if (x == arrival)
            {
                cout << name << " ARRIVALS" << endl;
                cout << "===========================" << endl;
                cout << "Flight   " << "From                " << " Scheduled" << " Expected "
                     << " Gate " << " Check-in " << " Comments " << endl;
                     for(int i=0; i<24;i++)
                       {
                        for(int j=0; j<60;j++)
                           {
                               if(slot[i][j] != nullptr )
                               {
                                   slot[i][j]->print_fun(slot[i][j]->get_scheduled());
                               }
                           }
                       }
            }
		if (x == departure)
            {
                cout << name << " DEPARTURES" << endl;
                cout << "===========================" << endl;
                cout << "Flight   " << "To                  " << " Scheduled" << " Expected "
                     << " Gate " << " Check-in " << " Comments " << endl;
                     for(int i=0; i<24;i++)
                        {
                            for(int j=0; j<60;j++)
                            {
                                if(slot[i][j] != nullptr )
                                {
                                    if(slot[i][j]->get_checkIn() == "")
                                        continue;

                                    slot[i][j]->print_fun(slot[i][j]->get_scheduled());
                                }
                            }
                        }
            }
	}
};

int main()
{
    LocalTime lc1, lc2, lc3, lc4, lc5, lc6, lc7;
    //LocalTime lc8;
    lc1.set_time(12,40);
    lc2.set_time(14,50);
    lc3.set_time(9,10);
    lc4.set_time(10,40);
    lc5.set_time(11,15);
    lc6.set_time(13,15);
    lc7.set_time(14,55);
    //lc8.set_time(16,55);
    Airport a("DUSSELDORF AIRPORT");
    Flight b1(arrival, "LH 2010  ", "Munich                  ", "A04 " );
    Flight b2(arrival, "EW 9347  ", "Manchester              ", "B04 " );
    a.schedule(lc1,&b1);
    a.schedule(lc2,&b2);
    a.print(arrival);
    Flight b3(departure, "AF 1307  ", "Paris                   ", "B51 ", "  192-194  ");
    Flight b4(departure, "SU 2537  ", "Moscow                  ", "C31 ", "  252-255  " );
    Flight b5(departure, "EW 9466  ", "London-Heathrow         ", "B35 ", "  151-170  " );
    Flight b6(departure, "LH 2011  ", "Munich                  ", "A40 ", "  115-120  " );
    Flight b7(departure, "XQ 959   ", "Izmir                   ", "C45 ", "  240-242  " );
    //Flight b8(departure, "YO 123   ", "Tokyo                   ", "D95 ", "  100-102  " );
    a.schedule(lc3,&b3);
    b3.set_comment("departed");
    b3.get_comment();
    a.schedule(lc4,&b4);
    b4.set_comment("boarding");
    b4.get_comment();
    a.schedule(lc5,&b5);
    a.schedule(lc6,&b6);
    b6.set_comment("Code Sharing");
    b6.get_comment();
    a.schedule(lc7,&b7);
    //a.schedule(lc8,&b8);
    a.print(departure);


	return 0;
}
