import pytest
import json

# Sample JSON data, parsed as a Python dictionary
data_json = """
{
  "company" : {
    "companyName" : "ABC",
    "employees" : [ {
      "name" : "Robinson",
      "age" : 25,
      "gender" : "Female",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "RobinsonLoc1",
        "street" : "Main Str",
        "zipCode" : "08831",
        "state" : "NJ"
      }, {
        "id" : "RobinsonLoc2",
        "street" : "Ocean Drive",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 2,
      "salary" : 220000.0
    }, {
      "name" : "Warner",
      "age" : 45,
      "gender" : "Male",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "WarnerLoc",
        "street" : "Maple Street",
        "zipCode" : "08817",
        "state" : "NJ"
      } ],
      "children" : 0,
      "salary" : 150000.0
    }, {
      "name" : "Stevens",
      "age" : 24,
      "gender" : "Male",
      "maritalStatus" : "Single",
      "minor" : false,
      "locations" : [ {
        "id" : "StevensLoc",
        "street" : "Regency Drive",
        "zipCode" : "08817",
        "state" : "NJ"
      } ],
      "children" : 0,
      "salary" : 135000.0
    }, {
      "name" : "White",
      "age" : 32,
      "gender" : "Female",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "WhiteLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 2,
      "salary" : 195000.0
    }, {
      "name" : "Smith",
      "age" : 46,
      "gender" : "Male",
      "maritalStatus" : "Single",
      "minor" : false,
      "locations" : [ {
        "id" : "SmithLoc",
        "street" : "Maple Street",
        "zipCode" : "90027",
        "state" : "CA"
      } ],
      "children" : 1,
      "salary" : 120000.0
    }, {
      "name" : "Green",
      "age" : 28,
      "gender" : "Female",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "GreenLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 3,
      "salary" : 140000.0
    }, {
      "name" : "Brown",
      "age" : 32,
      "gender" : "Male",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "BrownLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 2,
      "salary" : 65000.0
    }, {
      "name" : "Klaus",
      "age" : 54,
      "gender" : "Male",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "KlausLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 1,
      "salary" : 85000.0
    }, {
      "name" : "Houston",
      "age" : 47,
      "gender" : "Female",
      "maritalStatus" : "Single",
      "minor" : false,
      "locations" : [ {
        "id" : "HoustonLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 2,
      "salary" : 135000.0
    }, {
      "name" : "Long",
      "age" : 29,
      "gender" : "Male",
      "maritalStatus" : "Married",
      "minor" : false,
      "locations" : [ {
        "id" : "LongLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 3,
      "salary" : 40000.0
    }, {
      "name" : "Short",
      "age" : 22,
      "gender" : "Male",
      "maritalStatus" : "Single",
      "minor" : false,
      "locations" : [ {
        "id" : "ShortLoc",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      } ],
      "children" : 0,
      "salary" : 120000.0
    }, {
      "name" : "Doe",
      "age" : 21,
      "gender" : "Female",
      "maritalStatus" : "Single",
      "minor" : false,
      "locations" : [ {
        "id" : "DoeLoc1",
        "street" : "Green Road",
        "zipCode" : "33019",
        "state" : "FL"
      }, {
        "id" : "DoeLoc2",
        "street" : "Morgan Street",
        "zipCode" : "33020",
        "state" : "FL"
      }, {
        "id" : "DoeLoc3",
        "street" : "Lyric Ave",
        "zipCode" : "90027",
        "state" : "CA"
      } ],
      "children" : 1,
      "salary" : 210000.0
    } ],
    "selectedZipCode" : "08817"
  }
}
"""

def load_company_data():
    """Function to load the company data from JSON."""
    return json.loads(data_json)

def filter_employees_by_state(company_data, state):
    """Filter employees based on the state."""
    employees = company_data['company']['employees']
    return [emp for emp in employees if any(loc['state'] == state for loc in emp['locations'])]

def get_employees_by_zip_code(company_data, zip_code):
    """Get employees living in a specific zip code."""
    employees = company_data['company']['employees']
    return [emp for emp in employees if any(loc['zipCode'] == zip_code for loc in emp['locations'])]

@pytest.fixture
def company_data():
    return load_company_data()

def test_filter_employees_by_state(company_data):
    florida_employees = filter_employees_by_state(company_data, 'FL')
    assert len(florida_employees) == 9, "Expected 9 employees in Florida, but got a different number."

def test_get_employees_by_zip_code(company_data):
    employees_zip_08817 = get_employees_by_zip_code(company_data, '08817')
    assert len(employees_zip_08817) == 2, "Expected 2 employees in zip code 08817, but got a different number."