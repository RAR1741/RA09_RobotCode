#ifndef ACCELERATION_LIMITER_H__
#define ACCELERATION_LIMITER_H__

/*********************************************************************************
 * AccelerationLimiter
 * 
 * A class that controls and limits the change in a variable in between calls.
 * 
 * If you are of a calculus bent, it works like a governer on a diesel engine on 
 * an 18-wheeler. The truck gets to the same destination, but possibly slower than
 * it would without the governer. In the same way, through a derivative
 * transformation, this acceleration governer limits the change in velocity, rather
 * than position, per unit time. The target velocity will still be reached, but
 * possibly more slowly.
 *********************************************************************************/
//class AccelerationLimiter
//{
//public:
//	AccelerationLimiter(float unitTime, float maxChange);
//	~AccelerationLimiter();
//	
//	void FeedNewInput(float x);
//	float GetCorrectedVariable(void); // Will continue to return current
//									  // corrected value until state updated.
//	
//	void UpdateState(void);
//private:
//	
//	float maxChange;
//	float last_x;
//	float correct_x;
//	bool updated;
//	bool dirty;						// Need to update.
//	
//	
//};

#endif