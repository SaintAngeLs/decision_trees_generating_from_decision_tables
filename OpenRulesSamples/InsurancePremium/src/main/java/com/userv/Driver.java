/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.userv;

public class Driver {
	String	driverName;
	int		age;
	String	gender;
	String	maritalStatus;
	String 	state;
	String	ageCategory;
	String	eligibilityRating;
	boolean	convictedOfDUI;
	int		movingViolationsInLastTwoYears;
	int		numberOfAccidents;
	boolean	hasTakenTrainingFromSchool;
	boolean	hasTakenTrainingFromLicensedDriverTrainingCompany;
	boolean	hasTakenSeniorCitizenDriverRefresherCourse;
	boolean	hasTrainingCertification;
	String	drivingRecordCategory;
	double	driverPremium;
	
	public String getDriverName() {
        return driverName;
    }
    public void setDriverName(String driverName) {
        this.driverName = driverName;
    }
    public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	public String getGender() {
		return gender;
	}
	public void setGender(String gender) {
		this.gender = gender;
	}
	public String getMaritalStatus() {
		return maritalStatus;
	}
	public void setMaritalStatus(String maritalStatus) {
		this.maritalStatus = maritalStatus;
	}
	public String getState() {
		return state;
	}
	public void setState(String state) {
		this.state = state;
	}
	public String getAgeCategory() {
		return ageCategory;
	}
	public void setAgeCategory(String ageCategory) {
		this.ageCategory = ageCategory;
	}
	public String getEligibilityRating() {
		return eligibilityRating;
	}
	public void setEligibilityRating(String eligibilityRating) {
		this.eligibilityRating = eligibilityRating;
	}
	public boolean isConvictedOfDUI() {
		return convictedOfDUI;
	}
	public void setConvictedOfDUI(boolean convictedOfDUI) {
		this.convictedOfDUI = convictedOfDUI;
	}
	public int getMovingViolationsInLastTwoYears() {
		return movingViolationsInLastTwoYears;
	}
	public void setMovingViolationsInLastTwoYears(int movingViolationsInLastTwoYears) {
		this.movingViolationsInLastTwoYears = movingViolationsInLastTwoYears;
	}
	public int getNumberOfAccidents() {
		return numberOfAccidents;
	}
	public void setNumberOfAccidents(int numberOfAccidents) {
		this.numberOfAccidents = numberOfAccidents;
	}
	public boolean isHasTakenTrainingFromSchool() {
		return hasTakenTrainingFromSchool;
	}
	public void setHasTakenTrainingFromSchool(boolean hasTakenTrainingFromSchool) {
		this.hasTakenTrainingFromSchool = hasTakenTrainingFromSchool;
	}
	public boolean isHasTakenTrainingFromLicensedDriverTrainingCompany() {
		return hasTakenTrainingFromLicensedDriverTrainingCompany;
	}
	public void setHasTakenTrainingFromLicensedDriverTrainingCompany(
			boolean hasTakenTrainingFromLicensedDriverTrainingCompany) {
		this.hasTakenTrainingFromLicensedDriverTrainingCompany = hasTakenTrainingFromLicensedDriverTrainingCompany;
	}
	public boolean isHasTakenSeniorCitizenDriverRefresherCourse() {
		return hasTakenSeniorCitizenDriverRefresherCourse;
	}
	public void setHasTakenSeniorCitizenDriverRefresherCourse(
			boolean hasTakenSeniorCitizenDriverRefresherCourse) {
		this.hasTakenSeniorCitizenDriverRefresherCourse = hasTakenSeniorCitizenDriverRefresherCourse;
	}
	public boolean isHasTrainingCertification() {
		return hasTrainingCertification;
	}
	public void setHasTrainingCertification(boolean hasTrainingCertification) {
		this.hasTrainingCertification = hasTrainingCertification;
	}
	public String getDrivingRecordCategory() {
		return drivingRecordCategory;
	}
	public void setDrivingRecordCategory(String drivingRecordCategory) {
		this.drivingRecordCategory = drivingRecordCategory;
	}
	public double getDriverPremium() {
		return driverPremium;
	}
	public void setDriverPremium(double driverPremium) {
		this.driverPremium = driverPremium;
	}
	@Override
	public String toString() {
		return "\nDriver [name=" + driverName + ", age=" + age + ", gender=" + gender
				+ ", maritalStatus=" + maritalStatus + ", state=" + state
				+ ", ageCategory=" + ageCategory + ", eligibilityRating="
				+ eligibilityRating + ", convictedOfDUI=" + convictedOfDUI
				+ ", movingViolationsInLastTwoYears="
				+ movingViolationsInLastTwoYears + ", numberOfAccidents="
				+ numberOfAccidents + ", hasTakenTrainingFromSchool="
				+ hasTakenTrainingFromSchool
				+ ", hasTakenTrainingFromLicensedDriverTrainingCompany="
				+ hasTakenTrainingFromLicensedDriverTrainingCompany
				+ ", hasTakenSeniorCitizenDriverRefresherCourse="
				+ hasTakenSeniorCitizenDriverRefresherCourse
				+ ", hasTrainingCertification=" + hasTrainingCertification
				+ ", drivingRecordCategory=" + drivingRecordCategory 
				+ ", driverPremium=" + driverPremium 
				+ "]";
	} 
	
}
