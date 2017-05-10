#ifndef DATETIME__HPP
#define DATETIME__HPP

/**
 * \brief The DateTime class permit to have human comprehensive format for the timestamp.
 *        
 */
class DateTime{
	// Operations
public :
	/**
	 * \brief Class constructor
	 * \param s
	 */
	DateTime(unsigned char s=0, unsigned char mi=0, unsigned char h=0, unsigned char d=0, unsigned char mo=0, unsigned int y=0);

	/**
	 * \brief convert from timstamp (unsigned long int) to date time format
	 * \param timestamp (???)
	 * \return void
	 */
	void toDateTime (unsigned long int timestamp);

	unsigned long int getDateTime ();

private :
	/**
	 * \brief test if the year is leap or not
	 * \param year (???)
	 * \return bool
	 */
	bool isLeap (unsigned int year);

	// Attributes
private :
	/// second register
	unsigned char m_second;
	/// minute register
	unsigned char m_minute;
	/// hour register
	unsigned char m_hour;
	/// date number register
	unsigned char m_date;
	/// month register
	unsigned char m_month;
	/// year register
	unsigned int m_year;

};

#endif
