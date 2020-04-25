#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

void encode(char* file);

bool check(char c);

char exclusion[7] = {
	' ', '\t', '\n', '{', '}', ';', '.'
};

int list[11] = {
	0x11, 0x1B, 0x1F, 0x21, 0x27, 0x2F, 0x31, 0x39, 0x3B, 0x3F, 0x43
};

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Invalid Parameter\n");
		getchar();
		return 0;
	}

	if (strcmp(argv[1], "encode") == 0)
	{
		printf("\n");

		clock_t start, end;

		start = clock();
		encode(argv[2]);
		end = clock();

		printf("\nDone! %0.3lfs\n", (static_cast<double>(end) - start) / 1000);
		return 0;
	}
	else if (strcmp(argv[1], "decode") == 0)
	{
		return 0;
	}
	else
	{
		printf("Invalid Parameter\n");
		getchar();
		return 0;
	}

	
}

void encode(char* file)
{
	ifstream ifs;
	ifs.open(file, ifstream::in);

	string read, text = "";

	while (getline(ifs, read))
		text += (read + '\n');
		
	ifs.close();

	const int LEN = 20;
	const unsigned long long MAX = static_cast<unsigned long long>(text.length()) * 2;
	unsigned long long count = 0;
	unsigned long long dcount = 1;
	if (!(MAX < 100)) dcount = MAX / 100;
	int bar = 0;
	float tick = 100.f / LEN;
	float percent;

	for (size_t i = 0; i < text.length(); i++)
	{
		if (count % dcount == 0)
		{
			printf("\r%lld/%lld [", count, MAX);
			percent = static_cast<float>(count) / MAX * 100;
			bar = static_cast<int>(percent / tick);

			for (int i = 0; i < LEN; i++)
			{
				if (bar > i) printf("=");
				else printf(" ");
			}

			printf("] %0.1f%%", percent);
		}



		if (check(text[i])) {
			count++;
			continue;
		}

		bool flag = false;
		for (size_t j = i + 1; j < text.length(); j++)
		{
			if (check(text[j])) continue;
			else
			{
				text[i] = text[i] ^ text[j];
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			text[i] = text[i] ^ 127;
		}

		count++;
	}

	int j = 0;
	for (size_t i = 0; i < text.length(); i++)
	{
		if (count % dcount == 0)
		{
			printf("\r%lld/%lld [", count, MAX);
			percent = static_cast<float>(count) / MAX * 100;
			bar = static_cast<int>(percent / tick);

			for (int i = 0; i < LEN; i++)
			{
				if (bar > i) printf("=");
				else printf(" ");
			}

			printf("] %0.1f%%", percent);
		}



		if (check(text[i]))
		{
			if (j == (sizeof(list) / sizeof(int) - 1)) j = 0;
			else j++;

			count++;

			continue;
		}

		text[i] = text[i] ^ list[j];

		if (j == (sizeof(list) / sizeof(int) - 1)) j = 0;
		else j++;

		count++;
	}

	printf("\r%lld/%lld [", count, MAX);
	percent = static_cast<float>(count) / MAX * 100;
	bar = static_cast<int>(percent / tick);

	for (int i = 0; i < LEN; i++)
	{
		if (bar > i) printf("=");
		else printf(" ");
	}

	printf("] %0.1f%%", percent);



	ofstream out;
	out.open("output\\Broken " + string(file), ifstream::out);

	out << text;

	out.close();
}

bool check(char c)
{
	for (int i = 0; i < sizeof(exclusion) / sizeof(char); i++)
	{
		if (c == exclusion[i])
		{
			return true;
		}
	}

	return false;
}

