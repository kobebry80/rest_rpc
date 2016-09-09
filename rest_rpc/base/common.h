#pragma once

namespace timax{ namespace rpc 
{
	//result要么是基本类型，要么是结构体；当请求成功时，code为0, 如果请求是无返回类型的，则result为空; 
	//如果是有返回值的，则result为返回值。response_msg会序列化为一个标准的json串，回发给客户端。 
	//网络消息的格式是length+body，由4个字节的长度信息（用来指示包体的长度）加包体组成。 
	template<typename T, typename Tag = void>
	struct response_msg
	{
		int		code;
		T		result;
		Tag		tag;
		META(code, result, tag);
	};

	template <typename T>
	struct response_msg<T, void>
	{
		int code;
		T result; //json格式字符串，基本类型或者是结构体.
		META(code, result);
	};

	struct head_t
	{
		int16_t		code;
		int16_t		reserve;
		uint32_t	id;
		uint32_t	len;
	};

	enum class result_code : int16_t
	{
		OK = 0,
		FAIL = 1,
		EXCEPTION = 2,
		ARGUMENT_EXCEPTION = 3
	};

	enum class error_code 
	{
		OK,					// rpc正确
		FAIL,				// rpc调用失败
		TIMEOUT,			// rpc超时
		CANCEL,				// rpc主动取消
		BADCONNECTION,		// 连接故障
	};

	class client_error
	{
	public:
		client_error()
			: client_error(error_code::OK, "")
		{}

		client_error(error_code code, std::string message)
			: code_(code)
			, message_(std::move(message))
		{}

		error_code get_code() const
		{
			return code_;
		}

		void set_code(error_code code)
		{
			code_ = code;
		}

		std::string const& get_message() const
		{
			return message_;
		}

		void set_message(std::string message)
		{
			message_ = std::move(message);
		}

	private:
		error_code		code_;
		std::string		message_;
	};

	//
	enum class framework_type
	{
		DEFAULT = 0,
		ROUNDTRIP = 1,
		PUB = 2,
		SUB = 3,
	};

	enum class data_type
	{
		JSON = 0,
		BINARY = 1,
	};

	static std::atomic<std::uint64_t> g_succeed_count(0); //for test qps
} }

