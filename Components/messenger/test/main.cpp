#include <condition_variable>
#include <mutex>
#include <sstream>
#include <future>
#include <cassert>
#include <iterator>

#include <messenger/messenger.h>

class TestClient : public messenger::ILoginCallback, messenger::IMessagesObserver, messenger::IRequestUsersCallback
{
public:
	TestClient(const std::string& jid)
		: m_ready(false)
	{
		messenger::MessengerSettings settings;
		settings.serverUrl = "127.0.0.1";
		m_messenger = messenger::GetMessengerInstance(settings);
		
        messenger::SecurityPolicy securityPolicy;
        m_messenger->Login((jid + "@localhost").c_str(), "", securityPolicy, this);

		std::unique_lock<std::mutex> lock(m_mutex);
		while (!m_ready)
		{
			m_cv.wait(lock);
		}

		m_messenger->RegisterObserver(this);
	}

	~TestClient()
	{
		m_messenger->Disconnect();
	}

	void SendMessage(const std::string& recpt, const std::string& text)
	{
		messenger::MessageContent msg;
		msg.type = messenger::message_content_type::Text;
		std::copy(text.begin(), text.end(), std::back_inserter(msg.data));
		m_messenger->SendMessage(recpt + "@localhost", msg);
	}

	std::string ReceiveMessage()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		while (m_receivedMsg.empty())
		{
			m_cv.wait(lock);
		}

		std::string res = m_receivedMsg;
		m_receivedMsg.clear();
		return res;
	}

	messenger::UserList GetActiveUsers()
	{
		m_messenger->RequestActiveUsers(this);
		return m_userList.get_future().get();
	}

	void OnOperationResult(messenger::operation_result::Type result) override
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_ready = true;
		m_cv.notify_all();
	}

	void OnMessageStatusChanged(const messenger::MessageId& msgId, messenger::message_status::Type status) override
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_receivedMsg = "<error>";
		m_cv.notify_all();
	}

	void OnMessageReceived(const messenger::UserId& senderId, const messenger::Message& msg) override
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_receivedMsg.assign(reinterpret_cast<const char*>(&msg.content.data[0]), msg.content.data.size());
		m_cv.notify_all();
	}

	void OnOperationResult(messenger::operation_result::Type result, const messenger::UserList& users) override
	{
		m_userList.set_value(users);
	}

private:
	std::shared_ptr<messenger::IMessenger> m_messenger;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	bool m_ready;
	std::string m_receivedMsg;

	std::promise<messenger::UserList> m_userList;
};

void TestMessages()
{
	TestClient cl1("test1");
	TestClient cl2("test2");

	int seq = 1;
	while (seq <= 10)
	{
		std::stringstream msg;
		msg << "Message " << seq;
		cl1.SendMessage("test2", msg.str());
		assert(cl2.ReceiveMessage() == msg.str());
		seq++;
	}
}

void TestOfflineMessages()
{
	TestClient cl1("test1");
	cl1.SendMessage("test2", "Message 1");
	cl1.SendMessage("test2", "Message 2");

	TestClient cl2("test2");
	assert(cl2.ReceiveMessage().compare("Message 1") == 0);
	assert(cl2.ReceiveMessage().compare("Message 2") == 0);
}

void TestUserList()
{
	TestClient cl1("test1");
	messenger::UserList list = cl1.GetActiveUsers();
	assert(std::find_if(list.begin(), list.end(), [&](const messenger::User& user) -> bool
	{
		return user.identifier == "test1@localhost";
	}) != list.end());
}

int main(int argc, char* argv[])
{
	TestMessages();
	TestOfflineMessages();
	TestUserList();
	return 0;
}