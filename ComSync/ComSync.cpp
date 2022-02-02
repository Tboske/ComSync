#include "ComSync.h"

#include "iostream"
#include "sstream"
#include "string"
#include <thread>


ComSync::ComSync()
{
	m_Commands.emplace("help"			, [this] { return Help();						});
	m_Commands.emplace("disconnect"		, [this] { return Help();						});
	m_Commands.emplace("connect"		, [this] { return Help();						});
	m_Commands.emplace("startHost"		, [this] { return StartHost();					});
	m_Commands.emplace("exit"			, [this] { return m_ExitApplications = true;	});

	
}

ComSync::~ComSync()
{

}

void ComSync::Run()
{
	while (!m_ExitApplications)
	{
		string input;
		getline(cin, input);

		if (input.empty())
			continue;

		// get command and arguments
		stringstream ss(input);
		const istream_iterator<string> ssBegin(ss), ssEnd;
		m_Arguments = vector(ssBegin, ssEnd);

		// check if any command was found
		auto found = m_Commands.find(m_Arguments[0]);
		if (found == m_Commands.cend())
		{
			// no comment was found
			cout << '"' << m_Arguments[0] << "\" is not a valid command!\n";
			cout << "Type \"help\" to see all commands\n";
			continue;
		}

		// a command was found
		thread thr( found->second );
		thr.detach();

		// clear argument vector
		m_Arguments.clear();
	}

}

void ComSync::Help() const
{
	cout << "Available commands:\n";
	for (auto command : m_Commands)
		cout << " - " << command.first << '\n';
}

void ComSync::Connect()
{
	if (m_pNetwork != nullptr)
		cout << "Already in use!\n";

	m_pNetwork = new UdpClient();
	m_pNetwork->Run();
}

void ComSync::Disconnect()
{
	delete m_pNetwork;
	m_pNetwork = nullptr;
}

void ComSync::StartHost()
{
	if (m_pNetwork != nullptr)
		cout << "Already in use!\n";

	m_pNetwork = new UDPServer();
	m_pNetwork->Run();
}
