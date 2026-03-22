#include<bits/stdc++.h>
#include<chrono>
#include<mutex>

using namespace std;

class RateLimiter {

private:

	int bucketsize,timeframe,tokenfreq;

	map<string,int> tokensLeft;
	map<string,chrono::system_clock::time_point> startTimeTracker;
	map<string,std::unique_ptr<std::mutex>> userMutex;
	mutex gm;

public:

	RateLimiter(int bucketsize, int timeframe, int tokenfreq) {

		this->bucketsize = bucketsize;
		this->timeframe  = timeframe;
		this->tokenfreq  = tokenfreq;

	}


	bool ApiEndpoint(string userId) {

		gm.lock();
		auto FirstTimeEntry = startTimeTracker.find(userId);
		if(FirstTimeEntry==startTimeTracker.end()) {
			userMutex[userId] = std::make_unique<std::mutex>();
			tokensLeft[userId] =  bucketsize - 1;
			startTimeTracker[userId] = chrono::system_clock::now();
			gm.unlock();
			return true;
		}
		auto & mymutex = userMutex[userId];
		gm.unlock();

		mymutex->lock();

		int secondsdelay = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now()-FirstTimeEntry->second).count();

		int mytokensleft = tokensLeft[userId];

		if(secondsdelay<=timeframe) {

			if(mytokensleft>0) {

				tokensLeft[userId]--;

				mymutex->unlock();

				return true;
			}
			else {
				mymutex->unlock();

				return false;
			}

		}
		else {

			int windowsbetween = (secondsdelay-1)/timeframe;

			int newtokens = min(bucketsize,mytokensleft+(windowsbetween*tokenfreq)) - 1;

			chrono::seconds addseconds(windowsbetween * timeframe);

			FirstTimeEntry->second  += addseconds;

			tokensLeft[userId] = newtokens;

			mymutex->unlock();

			return true;

		}

	}


};



int main() {


	RateLimiter rl(10,1,5);

	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User2")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User2")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User2")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User2")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User2")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;
	cout<<rl.ApiEndpoint("User2")<<endl;
	cout<<rl.ApiEndpoint("User1")<<endl;


}
