/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.userv;

import java.util.Arrays;

public class Car {
	String		name;
	int			year;
	int			price;
	boolean		newCar;
	int			age;
	boolean		convertible;
	boolean		onTheTheftProbabilityList;
	String		type;
	String[]	features;
	boolean 	uninsuredMotoristCoverageIncluded;
	boolean		medicalCoverageIncluded;
	String		potentialOccupantInjuryRating;
	String		eligibilityRating;
	String 		potentialTheftRating;
	double		basePremium;
	double		autoDiscount;
	double		autoPremium;
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getYear() {
		return year;
	}
	public void setYear(int year) {
		this.year = year;
	}
	public int getPrice() {
		return price;
	}
	public void setPrice(int price) {
		this.price = price;
	}
	public boolean isConvertible() {
		return convertible;
	}
	public void setConvertible(boolean convertible) {
		this.convertible = convertible;
	}
	public boolean isOnTheTheftProbabilityList() {
		return onTheTheftProbabilityList;
	}
	public void setOnTheTheftProbabilityList(boolean onTheTheftProbabilityList) {
		this.onTheTheftProbabilityList = onTheTheftProbabilityList;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public String[] getFeatures() {
		return features;
	}
	public void setFeatures(String[] features) {
		this.features = features;
	}
	public String getPotentialOccupantInjuryRating() {
		return potentialOccupantInjuryRating;
	}
	public void setPotentialOccupantInjuryRating(
			String potentialOccupantInjuryRating) {
		this.potentialOccupantInjuryRating = potentialOccupantInjuryRating;
	}
	public String getEligibilityRating() {
		return eligibilityRating;
	}
	public void setEligibilityRating(String eligibilityRating) {
		this.eligibilityRating = eligibilityRating;
	}
	public String getPotentialTheftRating() {
		return potentialTheftRating;
	}
	public void setPotentialTheftRating(String potentialTheftRating) {
		this.potentialTheftRating = potentialTheftRating;
	}
	
	public boolean isNewCar() {
		return newCar;
	}
	public void setNewCar(boolean newCar) {
		this.newCar = newCar;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}
	public boolean isUninsuredMotoristCoverageIncluded() {
		return uninsuredMotoristCoverageIncluded;
	}
	public void setUninsuredMotoristCoverageIncluded(
			boolean uninsuredMotoristCoverageIncluded) {
		this.uninsuredMotoristCoverageIncluded = uninsuredMotoristCoverageIncluded;
	}
	public boolean isMedicalCoverageIncluded() {
		return medicalCoverageIncluded;
	}
	public void setMedicalCoverageIncluded(boolean medicalCoverageIncluded) {
		this.medicalCoverageIncluded = medicalCoverageIncluded;
	}
	public double getAutoDiscount() {
		return autoDiscount;
	}
	public void setAutoDiscount(double autoDiscount) {
		this.autoDiscount = autoDiscount;
	}
	public double getAutoPremium() {
		return autoPremium;
	}
	public void setAutoPremium(double autoPremium) {
		this.autoPremium = autoPremium;
	}
	public double getBasePremium() {
		return basePremium;
	}
	public void setBasePremium(double basePremium) {
		this.basePremium = basePremium;
	}
	@Override
	public String toString() {
		return "\nCar [name=" + name + ", year=" + year + ", price=" + price
				+ ", newCar=" + newCar + ", age=" + age + ", convertible="
				+ convertible + ", onTheTheftProbabilityList="
				+ onTheTheftProbabilityList + ", type=" + type + ", features="
				+ Arrays.toString(features)
				+ ", uninsuredMotoristCoverageIncluded="
				+ uninsuredMotoristCoverageIncluded
				+ ", medicalCoverageIncluded=" + medicalCoverageIncluded
				+ ", potentialOccupantInjuryRating="
				+ potentialOccupantInjuryRating + ", eligibilityRating="
				+ eligibilityRating + ", potentialTheftRating="
				+ potentialTheftRating 
				+ ", basePremium=" + basePremium
				+ ", autoDiscount=" + autoDiscount 
				+ ", autoPremium=" + autoPremium
				+ "]";
	}


}
