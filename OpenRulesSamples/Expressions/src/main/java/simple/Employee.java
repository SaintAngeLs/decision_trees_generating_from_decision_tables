/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package simple;

public class Employee {
	
	String name;
	double salary;
	boolean citizen;
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public double getSalary() {
		return salary;
	}
	public void setSalary(double salary) {
		this.salary = salary;
	}
	
	public boolean isCitizen() {
        return citizen;
    }
    public void setCitizen(boolean citizen) {
        this.citizen = citizen;
    }
    @Override
    public String toString() {
        return "Employee [name=" + name + ", salary=" + salary + ", citizen=" + citizen + "]";
    }

}
