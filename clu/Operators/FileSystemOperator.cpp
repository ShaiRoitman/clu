#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <map>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);
USING_NAMESPACE(boost::filesystem);


class FileSystemOperator : public clu::InputFileOperator {
public:
	FileSystemOperator()
	{
		m_regex1Operands = "([a-z]*)\\s+(.*)";
		m_regex2Operands = "([a-z]*)\\s+(.*)\\s+(.*)";
		m_regex2OperandsQuotes = "([a-z]*)\\s+(\".*\")\\s+(\".*\")";
		this->initOperators();
	}

	bool OnLineRead(string& line)
	{
		string op, arg1, arg2;
		int numArgs = this->readParams(line, op, arg1, arg2);
		if ( numArgs == 1 )
		{
			if (this->func1Ops.find(op) != this->func1Ops.end())
			{
				path path1 = arg1;
				auto func = this->func1Ops[op];
				func(path1);
			}
		}
		else if ( numArgs == 2 )
		{
			if (this->func2Ops.find(op) != this->func2Ops.end())
			{
				path path1 = arg1;
				path path2 = arg2;
				auto func = this->func2Ops[op];
				func(path1, path2);
			}
		}
		return true;
	}
private:
	void initOperators()
	{
		this->func1Ops["dirname"] = [=](path arg1)
		{
			boost::filesystem::path dirname = arg1.parent_path().string();
			m_OutputHandler->OutputLineFeed(dirname.string());
		};
		this->func1Ops["mkdir"] = [=](path arg1)
		{
			boost::system::error_code ec;
			boost::filesystem::create_directories(arg1, ec);
		};
		this->func1Ops["rmdir"] = [=](path arg1)
		{
			boost::system::error_code ec;
			boost::filesystem::remove(arg1, ec);
		};
		this->func1Ops["abs"] = [=](path arg1)
		{
			boost::filesystem::path path = arg1;
			boost::filesystem::path abs = boost::filesystem::absolute(path);
			m_OutputHandler->OutputLineFeed(abs.string());
		};
		this->func2Ops["mv"] = [=](path arg1, path arg2)
		{
			boost::system::error_code ec;
			boost::filesystem::rename(arg1, arg2, ec);
		};
		this->func2Ops["ln"] = [=](path arg1, path arg2)
		{
			boost::system::error_code ec;
			boost::filesystem::create_symlink(arg1, arg2, ec);
			if (ec.value() != 0)
			{
				m_OutputHandler->Output(string("failed to create link source [") + arg1.string() + string("] target [") + arg2.string() + string("]\n"));
			}
		};
	}
	int readParams(const string& line, string& op, string& arg1, string& arg2)
	{
		int retValue = 0;
		boost::match_results<string::const_iterator> matches;
		if (boost::regex_match(line, matches, m_regex2OperandsQuotes))
		{
			op = matches[1].str();
			arg1 = matches[2].str();
			arg2 = matches[3].str();
			retValue = 2;
		}
		else if (boost::regex_match(line, matches, m_regex2Operands))
		{
			op = matches[1].str();
			arg1 = matches[2].str();
			arg2 = matches[3].str();
			retValue = 2;
		}
		else if (boost::regex_match(line, matches, m_regex1Operands))
		{
			op = matches[1].str();
			arg1 = matches[2].str();
			retValue = 1;
		}

		return retValue;
	}

	boost::regex m_regex1Operands, m_regex2Operands, m_regex2OperandsQuotes;
	map<string, std::function<void(path)> > func1Ops;
	map<string, std::function<void(path, path)> > func2Ops;
};

REGISTER_PREDICATE("filesystem", FileSystemOperator)->SetHelp("Batch file system operations supporting ln, mkdir, rmdir, mv, dirname, abs");
