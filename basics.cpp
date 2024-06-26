#include <iostream>
#include <ql/quantlib.hpp>
#include "basics.h"

using namespace std;
using namespace QuantLib;

void date_class() {

	//1.Date Class
	Date date(31, March, 2015);
	cout << date << endl;

	cout << date.month() << "-" << date.dayOfMonth() << "-" << date.year() << endl;

	cout << date.weekday() << endl;

	cout << (date.weekday() == Tuesday);

	cout << "add a day     :" << date + 1 << endl;
	cout << "subtract a day:" << date - 1 << endl;
	cout << "add a week    :" << date + Period(1, Weeks) << endl;
	cout << "add a month   :" << date - Period(1, Months) << endl;
	cout << "add a year    :" << date + Period(1, Years) << endl;

	cout << (date == Date(31, March, 2015)) << endl;
	cout << (date > Date(31, March, 2015)) << endl;
	cout << (date < Date(1, April, 2015)) << endl;
	cout << (date != Date(1, April, 2015));
}

void calendar_class() {
	Date date(31, March, 2015);
	// to do
	Calendar us_calendar = UnitedStates(UnitedStates::Settlement);
	Calendar italy_calendar = Italy(Italy::Settlement);

	Period period(60, Days);
	Date raw_date = date + period;
	Date us_date = us_calendar.advance(date, period);
	Date italy_date = italy_calendar.advance(date, period);

	cout << "add 60 days:" << raw_date << endl;
	cout << "add 60 business days in US:" << us_date << endl;
	cout << "add 60 business days in Italy:" << italy_date << endl;


	auto us_busdays = us_calendar.businessDaysBetween(date, us_date);
	auto italy_busdays = italy_calendar.businessDaysBetween(date, italy_date);

	cout << "Buisness days US: " << us_busdays << endl;
	cout << "Business days Italy:" << italy_busdays << endl;

	//joint calendar 

	JointCalendar joint_calendar(us_calendar, italy_calendar);

	Date joint_date = joint_calendar.advance(date, period);
	auto joint_busdays = joint_calendar.businessDaysBetween(date, joint_date);

	cout << "add 60 business days in US-Italy:" << joint_date << endl;
	cout << "Business days US-Italy:" << joint_busdays << endl;

}

void print_schedule(Schedule schedule) {
	vector<Date> dates = schedule.dates();
	for (int i = 0; i < dates.size(); i++) {
		cout << i + 1 << " " << dates[i] << endl;
	}

}

void schedule_class() {
	Date effective_date(1, January, 2015);
	Date termination_date(1, January, 2016);
	Period tenor(Monthly);
	Calendar calendar = UnitedStates(UnitedStates::Settlement);
	BusinessDayConvention business_convention = Following;
	BusinessDayConvention termination_business_convention = Following;
	bool end_of_month = false;

	Schedule schedule(effective_date, termination_date, tenor, calendar, business_convention, termination_business_convention, DateGeneration::Backward, end_of_month);
	print_schedule(schedule);
	cout << "----------short stub in the front----------" << endl;
	//short stub in the front
	Date first_date(15, January, 2015);
	Schedule schedule2(effective_date, termination_date, tenor, calendar, business_convention, termination_business_convention, DateGeneration::Backward, end_of_month, first_date);
	print_schedule(schedule2);

	//short stub at the back
	cout << "----------short stub at the back----------" << endl;
	Date penultimate_date(15, December, 2015);
	Schedule schedule3(effective_date, termination_date, tenor, calendar, business_convention, termination_business_convention, DateGeneration::Forward, end_of_month, Date(), penultimate_date);
	print_schedule(schedule3);

	//long stub in the front
	cout << "----------long stub in the front----------" << endl;
	first_date = Date(1, February, 2015);
	effective_date = Date(15, December, 2014);
	termination_date = Date(1, January, 2016);
	Schedule schedule4(effective_date, termination_date, tenor, calendar, business_convention, termination_business_convention, DateGeneration::Backward, end_of_month, first_date);
	print_schedule(schedule4);

	//long stub at the back
	cout << "----------long stub at the back----------" << endl;
	effective_date = Date(15, January, 2015);
	termination_date = Date(15, January, 2016);
	penultimate_date = Date(1, December, 2015);
	Schedule schedule5(effective_date, termination_date, tenor, calendar, business_convention, termination_business_convention, DateGeneration::Forward, end_of_month, Date(), penultimate_date);
	print_schedule(schedule5);


	//build schedule by a list of Date
	cout << "----------build schedule by a list of Date----------" << endl;
	vector<Date> dates;
	dates.push_back(Date(2, January, 2015));    dates.push_back(Date(2, February, 2015));
	dates.push_back(Date(2, March, 2015));      dates.push_back(Date(1, April, 2015));
	dates.push_back(Date(1, May, 2015));        dates.push_back(Date(1, June, 2015));
	dates.push_back(Date(1, July, 2015));       dates.push_back(Date(3, August, 2015));
	dates.push_back(Date(1, September, 2015));  dates.push_back(Date(1, October, 2015));
	dates.push_back(Date(2, November, 2015));   dates.push_back(Date(2, December, 2015));
	dates.push_back(Date(4, January, 2016));

	Schedule schedule6(dates);
	print_schedule(schedule6);

}

void interest_rate() {
	Real annual_rate = 0.05;
	DayCounter dc = ActualActual(ActualActual::ISDA);
	Compounding compound_type = Compounded;
	Frequency frequency = Annual;
	InterestRate interest_rate(annual_rate, dc, compound_type, frequency);

	Real t = 2.0;
	//compoundFactor，复利因子，若为simple,则为1+rt
	cout << interest_rate.compoundFactor(t) << endl;
	cout << (1 + annual_rate) * (1.0 + annual_rate) << endl;

	cout << setprecision(10);
	cout << interest_rate.discountFactor(t) << endl;
	cout << 1.0 / interest_rate.compoundFactor(t) << endl;

	Frequency new_frequency = Semiannual;
	InterestRate new_interest_rate = interest_rate.equivalentRate(compound_type, new_frequency, t);
	Real new_annual_rate = new_interest_rate.rate();
	cout << new_annual_rate << endl;

	cout << interest_rate.discountFactor(t) << endl;
	cout << new_interest_rate.discountFactor(t) << endl;

}

// take a textbook example : a European Option
void instruments() {
	Date today(7, March, 2014);
	Settings::instance().evaluationDate() = today;
	auto payOff = ext::make_shared<PlainVanillaPayoff>(Option::Call, 100.0);
	auto exercise = ext::make_shared<EuropeanExercise>(Date(7, June, 2014));

	EuropeanOption option(payOff, exercise);

	auto u = ext::make_shared<SimpleQuote>(100.0);
	auto r = ext::make_shared<SimpleQuote>(0.01);
	auto sigma = ext::make_shared<SimpleQuote>(0.20);

	Handle<YieldTermStructure> riskFreeCurve(ext::make_shared<FlatForward>(0, TARGET(), Handle<Quote>(r), Actual360()));

	Handle<BlackVolTermStructure> volatility(ext::make_shared<BlackConstantVol>(0, TARGET(), Handle<Quote>(sigma), Actual360()));

	auto process = ext::make_shared<BlackScholesProcess>(Handle<Quote>(u), riskFreeCurve,
		volatility);
	auto engine = ext::make_shared<AnalyticEuropeanEngine>(process);

	option.setPricingEngine(engine);

	cout << option.NPV() << endl;
	cout << option.delta() << endl;
	cout << option.gamma() << endl;
	cout << option.vega() << endl;

	//market changes
	cout << "----------market changes----------" << endl;

	u->setValue(105.0);
	cout << option.NPV() << endl;
	r->setValue(0.01);
	sigma->setValue(0.20);

	cout << option.NPV() << endl;

	r->setValue(0.03);

	cout << option.NPV() << endl;

	sigma->setValue(0.25);

	cout << option.NPV() << endl;

	//date changes
	cout << "----------date changes----------" << endl;
	u->setValue(105.0);
	r->setValue(0.01);
	sigma->setValue(0.20);
	cout << option.NPV() << endl;
	Settings::instance().evaluationDate() = Date(7, April, 2014);
	cout << option.NPV() << endl;

	//other pricing methods
	cout << "----------pricing mothod changes----------" << endl;
	Settings::instance().evaluationDate() = today;
	cout << option.NPV() << endl;

	Handle<YieldTermStructure> flatForward(ext::make_shared<FlatForward>(0, TARGET(), 0.0, Actual360()));

	auto process2 = ext::make_shared<HestonProcess>(Handle<YieldTermStructure>(riskFreeCurve),
		Handle<YieldTermStructure>(flatForward), Handle<Quote>(u), 0.04, 0.1, 0.01, 0.05, -0.75);

	auto model = ext::make_shared<HestonModel>(process2);

	auto engine2 = ext::make_shared<AnalyticHestonEngine>(model);
	option.setPricingEngine(engine2);
	cout << option.NPV() << endl;

	//lazy recalculation

	auto engine3 = MakeMCEuropeanEngine<PseudoRandom>(process)
		.withSteps(20)
		.withSamples(250000);

	option.setPricingEngine(engine3);
	cout << option.NPV() << endl;

	u->setValue(104.0);
	cout << option.NPV() << endl;

}

void numerical_greeks() {
	Date today(8, October, 2014);
	Settings::instance().evaluationDate() = today;
	// use a knock-in barrier option as example
	auto payoff = ext::make_shared<PlainVanillaPayoff>(Option::Call, 100.0);
	auto exercise = ext::make_shared<EuropeanExercise>(Date(8, January, 2015));
	BarrierOption option(Barrier::UpIn, 120.0, 0.0, payoff, exercise);

	// in this example, the market data are the underlying value, the risk-free rate and the volatility
	auto u = ext::make_shared<SimpleQuote>(100.0);
	auto r = ext::make_shared<SimpleQuote>(0.01);
	auto sigma = ext::make_shared<SimpleQuote>(0.20);

	Handle<YieldTermStructure> riskFreeCurve(ext::make_shared<FlatForward>(0, TARGET(), Handle<Quote>(r), Actual360()));
	Handle<BlackVolTermStructure> volatility(ext::make_shared<BlackConstantVol>(0, TARGET(), Handle<Quote>(sigma), Actual360()));

	auto process = ext::make_shared<BlackScholesProcess>(Handle<Quote>(u), riskFreeCurve, volatility);

	option.setPricingEngine(ext::make_shared<AnalyticBarrierEngine>(process));

	// now we can ask the option for its value
	cout << option.NPV() << endl;

	//the pricing engine we use doesn't provide the delta, so the code below will throw exception 
	try {
		cout << option.delta() << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}


	Real u0 = u->value();
	Real h = 0.01;
	Real p0 = option.NPV();
	u->setValue(u0 + h);
	Real p_plus = option.NPV();
	u->setValue(u0 - h);
	Real p_minus = option.NPV();

	cout << "p_plus: " << p_plus << "      p_minus: " << p_minus << endl;
	Real delta = (p_plus - p_minus) / (2 * h);
	Real gamma = (p_plus - 2 * p0 + p_minus) / (h * h);
	cout << "delta: " << delta << endl << "gamma: " << gamma << endl;


	// then we calculate Rho and Vega
	// the sensitivity of an option to changes in the level of interest rates, as captured by rho
	r->setValue(0.01);
	u->setValue(100.0);
	Real r0 = r->value();
	h = 0.0001;
	r->setValue(r0 + h);
	p_plus = option.NPV();
	r->setValue(r0);
	p0 = option.NPV();
	Real rho = (p_plus - p0) / h;


	//Vega is a measure of the change in the price of the option for every 1% change in volatility
	Real sigma0 = sigma->value();
	sigma->setValue(sigma0 + h);
	p_plus = option.NPV();
	sigma->setValue(sigma0);
	p0 = option.NPV();
	Real vega = (p_plus - p0) / h;

	cout << "rho: " << rho << endl << "vega: " << vega << endl;

	// Theta
	Settings::instance().evaluationDate() = today + 1;
	Real p1 = option.NPV();
	h = 1.0 / 365;
	Real theta = (p1 - p0) / h;
	cout << "theta: " << theta << endl;

}

void market_quote() {
	Date today(17, October, 2016);
	Settings::instance().evaluationDate() = today;
	vector<int> length = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 };
	vector<Real> coupon = { 0.02, 0.0225, 0.025, 0.0275, 0.03, 0.0325, 0.035, 0.0375, 0.04, 0.0425, 0.045, 0.0475, 0.05, 0.0525, 0.055 };

	Calendar calendar = TARGET();
	Date settlement = calendar.advance(today, 3, Days);
	vector<ext::shared_ptr<SimpleQuote>> quotes;
	vector<ext::shared_ptr<BondHelper>> helpers;


	for (int i = 0; i < length.size(); i++) {
		Date maturity = calendar.advance(settlement, length[i], Years);
		Schedule schedule(settlement, maturity, Period(Annual), calendar, ModifiedFollowing, ModifiedFollowing, DateGeneration::Backward, false);
		ext::shared_ptr<SimpleQuote> quote = ext::make_shared<SimpleQuote>(100.0);
		quotes.push_back(quote);
		auto helper = ext::make_shared<FixedRateBondHelper>(Handle<Quote>(quote), 3, 100.0, schedule, vector<Rate>(1, coupon[i]), SimpleDayCounter(), ModifiedFollowing);
		helpers.push_back(helper);
	}

	auto curve = ext::make_shared<FittedBondDiscountCurve>(0, calendar, helpers, SimpleDayCounter(), NelsonSiegelFitting());

	Schedule bondSchedule(today, calendar.advance(today, 15, Years), Period(Semiannual), calendar, ModifiedFollowing, ModifiedFollowing, DateGeneration::Backward, false);
	FixedRateBond bond(3, 100.0, bondSchedule, vector<Real>(1, 0.04), Actual360());
	bond.setPricingEngine(ext::make_shared<DiscountingBondEngine>(Handle<YieldTermStructure>(curve)));
	//auto cashflows = bond.cashflows();
	//for (int i = 0; i < cashflows.size(); i++) {
	//	cout << cashflows[i]->date() << "  " << cashflows[i]->amount() << "  " << curve->discount(cashflows[i]->date()) << endl;
	//}
	cout << bond.cleanPrice() << endl;

	//now let's add an observer that checks whether the bond is out of date, and if so recalculates the bond and outputs its new price
	//the changes will come from the market quotes, but the observer doesn't need to be concerned with that and only registers with the object it's
	// ultimately interested in 

	quotes[2]->setValue(101.0);
	cout << bond.cleanPrice() << endl;
	bond.freeze();
	quotes[2]->setValue(100.0);
	cout << bond.cleanPrice() << endl;
	bond.unfreeze();
	cout << bond.cleanPrice() << endl;

}







