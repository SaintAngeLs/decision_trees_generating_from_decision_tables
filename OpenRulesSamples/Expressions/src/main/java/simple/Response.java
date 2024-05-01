/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package simple;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;

public class Response {
	String		id;
	String[]	regions;
	Date[]		dates;
	int[]		terms;
	double[]	amounts;
	double[] 	externalAmounts;
	ArrayList<String>	books;
	ArrayList<String>	libraryBooks;
	Employee[]	employees;

    public Response() {
		super();
		this.regions = new String[0];
		this.dates = new Date[0];
		this.terms = new int[0];
		this.amounts = new double[0];
		this.externalAmounts = new double[0];
		this.books = new ArrayList<String>();
		this.libraryBooks = new ArrayList<String>();
		this.employees = new Employee[0];
	}
	
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}
	public String[] getRegions() {
		return regions;
	}
	public void setRegions(String[] regions) {
		this.regions = regions;
	}
	public Date[] getDates() {
		return dates;
	}
	public void setDates(Date[] dates) {
		this.dates = dates;
	}
	public int[] getTerms() {
		return terms;
	}
	public void setTerms(int[] terms) {
		this.terms = terms;
	}
	public double[] getAmounts() {
		return amounts;
	}
	public void setAmounts(double[] amounts) {
		this.amounts = amounts;
	}
	
	public double[] getExternalAmounts() {
		return externalAmounts;
	}
	public void setExternalAmounts(double[] externalAmounts) {
		this.externalAmounts = externalAmounts;
	}
	
	public ArrayList<String> getBooks() {
		return books;
	}

	public void setBooks(ArrayList<String> books) {
		this.books = books;
	}

	public ArrayList<String> getLibraryBooks() {
        return libraryBooks;
    }

    public void setLibraryBooks(ArrayList<String> libraryBooks) {
        this.libraryBooks = libraryBooks;
    }

    public Employee[] getEmployees() {
		return employees;
	}

	public void setEmployees(Employee[] employees) {
		this.employees = employees;
	}

	@Override
	public String toString() {
		return "Response [id=" + id + "\n regions=" + Arrays.toString(regions) + "\n dates=" + Arrays.toString(dates)
				+ "\n terms=" + Arrays.toString(terms) + "\n amounts=" + Arrays.toString(amounts) 
				+ "\n books=" + books 
				+ "\n employees=" + Arrays.toString(employees) ;
	}
	
	


}
