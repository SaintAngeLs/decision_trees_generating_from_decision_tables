/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package aggregated.values.lists;

import java.util.ArrayList;
import java.util.Arrays;

public class Department {
	String	departmentName;
	ArrayList<Employee>	employees;
	ArrayList<Employee>	RICHEmployees;
	String[] names;
	int[] salaries;
	int	minSalary;
	int	maxSalary;
	double	totalSalary;
	int	numberOfEmployees;
	double	averageSalary;
	int	numberOfHighPaidEmployees;
	ArrayList<Employee>	women;
	ArrayList<Employee>  men;
	
	public Department() {
	    this(null);
	}
	
	public Department(String departmentName) {
		this.departmentName = departmentName;
		employees = new ArrayList<Employee>();
		women = new ArrayList<Employee>();
		men = new ArrayList<Employee>();
		names = new String[0];
		salaries = new int[0];
		RICHEmployees = new ArrayList<Employee>();
	}
	
	public String getDepartmentName() {
		return departmentName;
	}
	public void setDepartmentName(String departmentName) {
		this.departmentName = departmentName;
	}
	public ArrayList<Employee> getEmployees() {
		return employees;
	}
	public void setEmployees(ArrayList<Employee> employees) {
		this.employees = employees;
	}
	public ArrayList<Employee> getRICHEmployees() {
		return RICHEmployees;
	}
	public void setRICHEmployees(ArrayList<Employee> richEmployees) {
		this.RICHEmployees = richEmployees;
	}
	public int getMinSalary() {
		return minSalary;
	}
	public void setMinSalary(int minSalary) {
		this.minSalary = minSalary;
	}
	public int getMaxSalary() {
		return maxSalary;
	}
	public void setMaxSalary(int maxSalary) {
		this.maxSalary = maxSalary;
	}
	public double getTotalSalary() {
		return totalSalary;
	}
	public void setTotalSalary(double totalSalary) {
		this.totalSalary = totalSalary;
	}
	public int getNumberOfEmployees() {
		return numberOfEmployees;
	}
	public void setNumberOfEmployees(int numberOfEmployees) {
		this.numberOfEmployees = numberOfEmployees;
	}
	public double getAverageSalary() {
		return averageSalary;
	}
	public void setAverageSalary(double averageSalary) {
		this.averageSalary = averageSalary;
	}
	public int getNumberOfHighPaidEmployees() {
		return numberOfHighPaidEmployees;
	}
	public void setNumberOfHighPaidEmployees(int numberOfHighPaidEmployees) {
		this.numberOfHighPaidEmployees = numberOfHighPaidEmployees;
	}
	
	public int[] getSalaries() {
		return salaries;
	}
	public void setSalaries(int[] salaries) {
		this.salaries = salaries;
	}
	
	public String[] getNames() {
		return names;
	}
	public void setNames(String[] names) {
		this.names = names;
	}
	
	
	
	public ArrayList<Employee> getWomen() {
		return women;
	}

	public void setWomen(ArrayList<Employee> women) {
		this.women = women;
	}

	public ArrayList<Employee> getMen() {
		return men;
	}

	public void setMen(ArrayList<Employee> men) {
		this.men = men;
	}

	@Override
	public String toString() {
		return "Department [department=" + departmentName + "\nemployees=" + Arrays.toString(employees.toArray())
				+ "\nwomen=" + women 
				+ "\nmen=" + men 
				+ "\nrichEmployees=" + Arrays.toString(RICHEmployees.toArray()) 
				+ "\nnames=" + Arrays.toString(names)
				+ "\nsalaries=" + Arrays.toString(salaries)
				+ "\nminSalary=" + minSalary + ", maxSalary=" + maxSalary
				+ ", totalSalary=" + totalSalary + ", numberOfEmployees=" + numberOfEmployees + ", averageSalary="
				+ averageSalary + ", numberOfHighPaidEmployees=" + numberOfHighPaidEmployees + "]"
				;
	}
	
	


}
