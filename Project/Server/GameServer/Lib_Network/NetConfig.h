#pragma once

namespace LibNet
{
	struct NetConfig
	{
		char		_ip[32];
		int			_port;
		unsigned	_maxSessionNum;
		unsigned	_maxSessionBufferSize;
		int			_backLog;
		unsigned	_clientBufferSize;
		unsigned	_bucketSize;
	};
}