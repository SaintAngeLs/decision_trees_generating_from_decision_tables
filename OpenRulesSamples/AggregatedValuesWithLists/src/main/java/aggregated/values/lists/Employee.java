/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package aggregated.values.lists;

public class Employee {
	String	name;
	int	age;
	String	gender;
	String	maritalStatus;
	int	salary;
	String	wealthCategory;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
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
	public int getSalary() {
		return salary;
	}
	public void setSalary(int salary) {
		this.salary = salary;
	}
	public String getWealthCategory() {
		return wealthCategory;
	}
	public void setWealthCategory(String wealthCategory) {
		this.wealthCategory = wealthCategory;
	}
	@Override
	public String toString() {
		return "Employee [name=" + name + ", age=" + age + ", gender=" + gender + ", maritalStatus=" + maritalStatus
				+ ", salary=" + salary + ", wealthCategory=" + wealthCategory + "]";
	}
	
	


}
