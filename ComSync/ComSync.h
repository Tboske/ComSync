#pragma once
#include <functional>
#include <unordered_map>
#include <cctype>

#include "UDPNetwork.h"

using namespace std;

class ComSync final
{
public:
	ComSync();
	~ComSync();

	void Run();

private:
	struct CaseInsensitiveCompare
	{
		static bool CompareChar(const char& c1, const char& c2)
		{
			if (c1 == c2)
				return true;
			if (toupper(c1) == toupper(c2))
				return true;
			return false;
		}
		bool operator()(const string& a, const string& b) const
		{
			return ((a.size() == b.size()) &&
				equal(a.begin(), a.end(), b.begin(), &CompareChar));
		}
	};
	typedef unordered_map<
		string, function<void()>, hash<string>, CaseInsensitiveCompare
	> CommandMap;

	CommandMap m_Commands;
	vector<string> m_Arguments;
	bool m_ExitApplications = false;


	UDPBase* m_pNetwork = nullptr;

	

private: //Functions
	void Help() const;
	void Connect();
	void Disconnect();
	void StartHost();


};

