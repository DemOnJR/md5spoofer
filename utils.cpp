#include <Windows.h>

char* stristr(char* source, char* substr)
{
	if ((source == nullptr) || (*source == '\0') || (substr == nullptr) || (*substr == '\0'))
		return nullptr;

	int substrLen = strlen(substr);

	while (*source != '\0')
	{
		if ((*source & ~0x20) == (*substr & ~0x20)) // do quick uppercase
		{
			if (!_strnicmp(source, substr, substrLen))
				return source;
		}

		source++;
	}

	return nullptr;
}

bool TryStrToInt(char* value, int &result)
{
	if ((value == nullptr) || (*value == '\0'))
		return false;

	bool isSigned = *value == '-';

	if (isSigned || *value == '+')
		value++;

	register int i = 0;

	for (;;)
	{
		char c = *value;
		if (c == '\0')
			break;

		if (c < '0' || c > '9')
			return false;

		i = i * 10 + (c - '0');
		value++;
	}

	if (isSigned)
		i = -i;

	result = i;

	return true;
}

void FormatFilePath(char *src, char *dest)
{
	if ((src == nullptr) || (*src == '\0') || (dest == nullptr))
		return;

	if (src != dest)
		strcpy(dest, src);

	char *p = dest;
	char *p2; // for later

	while (*p != '\0')
	{
		if (*p == '/')
			*p = '\\';

		p++;
	}

	p = strstr(dest, "\\\\");
	if (p != nullptr)
	{
		p2 = p;
		while (*p2 == '\\')
			p2++;

		p2--;
		strcpy(p, p2);
	}

	while (true)
	{
		p = strstr(dest, "..");

		if (p != nullptr)
		{
			p2 = p;
			while (*p2 != '\\')
				p2--;

			p2--;

			while (*p2 != '\\')
				p2--;

			p += 2;

			strcpy(p2, p);
			p = p2;
		}
		else
			break;
	}
}

void FormatFilePath(char *src)
{
	FormatFilePath(src, src);
}