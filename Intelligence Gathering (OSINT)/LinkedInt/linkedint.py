import sys
from pyfiglet import Figlet
custom_fig = Figlet(font='graffiti')
print(custom_fig.renderText('LinkedInt'))
from linkedin_api import Linkedin

def get_input():
    # Ask for full name or email address or both
    full_name = input("Enter full name (leave blank if not available): ").strip()
    email = input("Enter email address (leave blank if not available): ").strip()
    
    return full_name, email

def search_by_email(api, email):
    # Search using email by attempting to search people with email as keyword (if supported)
    print(f"Searching LinkedIn profiles for email: {email}")
    profiles = api.search_people(keywords=email, limit=20) # Set your desired output limit
    return profiles

def search_by_full_name(api, full_name):
    # Search using full name as keywords
    print(f"Searching LinkedIn profiles for full name: {full_name}")
    profiles = api.search_people(keywords=full_name, limit=20) # Set your desired output limit
    return profiles

def main():
    # Hardcoded LinkedIn credentials (replace with actual credentials)
    username = "example@example.com"
    password = "Admin@123"

    try:
        api = Linkedin(username, password)
    except Exception as e:
        print(f"Error logging in: {e}")
        sys.exit(1)

    # Get user input (either full name, email, or both)
    full_name, email = get_input()

    profiles = []

    if full_name and email:
        # If both full name and email are provided, try searching by both
        print(f"Searching using both full name and email...")
        profiles = search_by_full_name(api, full_name) + search_by_email(api, email)
    elif full_name:
        # If only full name is provided, search by full name
        profiles = search_by_full_name(api, full_name)
    elif email:
        # If only email is provided, attempt to search by email
        profiles = search_by_email(api, email)
    else:
        print("Please enter either a full name or an email address.")
        sys.exit(1)

    # Display full name, latest company and position, latest education, and profile URL
    if profiles:
        print(f"Found {len(profiles)} profiles:")
        for profile in profiles:
            name = profile.get('name', 'N/A')
            urn_id = profile.get('urn_id', None)

            if urn_id:
                try:
                    # Fetch detailed profile information using urn_id
                    detailed_profile = api.get_profile(urn_id)

                    # Extract profile fields
                    public_id = detailed_profile.get('public_id', None)
                    profile_url = f"https://www.linkedin.com/in/{public_id}" if public_id else "Not available"

                    # Safely extract latest experience (company and position)
                    experience = detailed_profile.get('experience', [])
                    if experience:
                        latest_experience = experience[0]
                        company = latest_experience.get('companyName', 'N/A')
                        position = latest_experience.get('title', 'N/A')
                    else:
                        company = 'Not available'
                        position = 'Not available'

                    # Safely extract latest education
                    education = detailed_profile.get('education', [])
                    if education:
                        latest_education = education[0]
                        school = latest_education.get('schoolName', 'N/A')
                        degree = latest_education.get('degreeName', 'N/A')
                    else:
                        school = 'Not available'
                        degree = 'Not available'

                    # Display extracted information
                    print(f"Name: {name}")
                    print(f"Profile URL: {profile_url}")
                    print(f"Latest Company: {company}")
                    print(f"Position: {position}")
                    print(f"Latest Education: {school}, {degree}\n")
                except Exception as e:
                    print(f"Error fetching detailed profile for {name}: {e}")
            else:
                print(f"Name: {name}\nProfile URL: Not available\n")
    else:
        print("No profiles found.")

if __name__ == "__main__":
    main()
