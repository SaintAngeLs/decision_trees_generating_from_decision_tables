/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package hello.java;

public class Customer {
	
	String name;
	String gender;
	String maritalStatus;
	int    currentHour; 
	String greeting;
	String salutation;
	String helloStatement;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
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
	public int getCurrentHour() {
		return currentHour;
	}
	public void setCurrentHour(int currentHour) {
		this.currentHour = currentHour;
	}
	public String getGreeting() {
		return greeting;
	}
	public void setGreeting(String greeting) {
		this.greeting = greeting;
	}
	public String getSalutation() {
		return salutation;
	}
	public void setSalutation(String salutation) {
		this.salutation = salutation;
	}
	public String getHelloStatement() {
		return helloStatement;
	}
	public void setHelloStatement(String helloStatement) {
		this.helloStatement = helloStatement;
	}
	@Override
	public String toString() {
		return "Customer [name=" + name + ", gender=" + gender + ", maritalStatus=" + maritalStatus + ", currentHour="
				+ currentHour 
				+ ", helloStatement=" + helloStatement + "]";
	}

}
