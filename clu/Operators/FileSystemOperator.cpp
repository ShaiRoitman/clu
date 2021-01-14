#include "InputFileOperator.h"
#include "CommandLineHandlers/PredicateCommandLineHandler.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

USING_NAMESPACE(std);
USING_NAMESPACE(clu);

class FileSystemOperator : public clu::InputFileOperator {
public:
	FileSystemOperator()
	{
		m_regex1Operands = "([a-z]*)\\s+(.*)";
		m_regex2Operands = "([a-z]*)\\s+(.*)\\s+(.*)";
		m_regex2OperandsQuotes = "([a-z]*)\\s+(\".*\")\\s+(\".*\")";
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
	bool OnLineRead(string& line)
	{
		string op, arg1, arg2;
		if (this->readParams(line, op, arg1, arg2) != 0)
		{
			if (op == "ln")
			{
				boost::system::error_code ec;
				boost::filesystem::path source = arg1;
				boost::filesystem::path target = arg2;
				boost::filesystem::create_symlink(source, target, ec);
				if (ec.value() != 0)
				{
					m_OutputHandler->Output(string("failed to create link source [") + source.string() + string("] target [") + target.string() + string("]\n"));
				}
			}
			else if (op == "mkdir")
			{
				boost::system::error_code ec;
				boost::filesystem::path dir = arg1;
				boost::filesystem::create_directory(dir, ec);
			}
			else if (op == "rmdir")
			{
				boost::system::error_code ec;
				boost::filesystem::path dir = arg1;
				boost::filesystem::remove(dir, ec);
			}
			else if (op == "mv")
			{
				boost::system::error_code ec;
				boost::filesystem::path source = arg1;
				boost::filesystem::path target = arg2;
				boost::filesystem::rename(arg1, arg2, ec);
			}
			else if (op == "dirname")
			{
				boost::system::error_code ec;
				boost::filesystem::path path = arg1;
				boost::filesystem::path dirname = path.parent_path().string();
				m_OutputHandler->OutputLineFeed(dirname.string());
			}
			else if (op == "abs")
			{
				boost::system::error_code ec;
				boost::filesystem::path path = arg1;
				boost::filesystem::path abs = boost::filesystem::absolute(path, ec);
				m_OutputHandler->OutputLineFeed(abs.string());
			}
		}
		return true;
	}
private:
	boost::regex m_regex1Operands, m_regex2Operands, m_regex2OperandsQuotes;
};

REGISTER_PREDICATE("filesystem", FileSystemOperator)->SetHelp("Batch file system operations supporting ln, mkdir, rmdir, mv, dirname, abs");
