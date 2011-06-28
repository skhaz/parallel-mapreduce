
#include <iostream>
#include <string>
#include <list>

#include <boost/pool/pool_alloc.hpp>
#include <boost/functional/hash.hpp>
#include <boost/tr1/unordered_map.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

namespace { const int num_threads = 4; }

typedef std::basic_string<char, std::char_traits<char>, boost::fast_pool_allocator<char> > fast_string;
typedef boost::iostreams::mapped_file_source::size_type mmap_size_t;
typedef boost::shared_ptr<boost::thread> thread_ptr;
typedef boost::unordered_multimap<
	fast_string, std::size_t,
	boost::hash<fast_string>,
	std::equal_to<fast_string>,
	boost::fast_pool_allocator<std::pair<fast_string, std::size_t> >
> fast_unorderedmap;

class worker
{
	public:
		worker() { }

		worker(mmap_size_t start, mmap_size_t size, const boost::iostreams::mapped_file_source& source)
		{
		}

		void run()
		{
			std::cout << "foobar " << std::endl;
		}
};

int main(int argc, char** argv)
{
	using std::cout;
	using std::endl;

	try
	{
		using boost::iostreams::mapped_file_source;

		std::vector<thread_ptr> threads;

		mapped_file_source source(argv[1]);
		mmap_size_t piece_size = source.size() / num_threads;
		mmap_size_t rest_size  = source.size() % num_threads;

		for (int i = num_threads; i--;)
		{
			boost::shared_ptr<worker> w(new worker(piece_size * i, piece_size, source));
			threads.push_back(thread_ptr(new boost::thread(boost::bind(&worker::run, w))));
		}

		std::for_each(threads.begin(), threads.end(), bind(&boost::thread::join, _1));
	}

	catch (std::ios::failure& fail)
	{
		cout << fail.what() << endl;
	}

	return 0;
}

