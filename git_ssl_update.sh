#!/bin/bash

# Function to check and update Git configuration
check_git_config() {
    local ssl_ca_info=$(git config --global --get http.sslcainfo)
    if [[ -n "$ssl_ca_info" ]]; then
        echo "http.sslcainfo already set: $ssl_ca_info"
    else
        echo "Setting http.sslcainfo to the default certificate bundle..."
        git config --global http.sslcainfo /etc/ssl/certs/ca-certificates.crt
    fi
}

# Function to disable SSL verification in Git (not recommended)
disable_ssl_verification() {
    echo "WARNING: Disabling SSL verification. Only use this in a trusted environment!"
    git config --global http.sslVerify false
}

# Function to update Git to use SSH for remote operations
use_ssh_for_remote() {
    echo "Switching to SSH for remote operations..."
    git remote set-url origin git@github.com:Salem-1/WebServe_42_Abu_Dhabi.git
}

# Main function to execute the script
main() {
    echo "Running Git SSL Certificate Fix Script..."

    # Update Git
    echo "Step 1: Updating Git..."
    git --version

    # Check system date and time
    echo "Step 2: Checking system date and time..."
    date

    # Check and update Git configuration
    echo "Step 3: Checking Git configuration..."
    check_git_config

    # Attempt to push to the remote repository
    echo "Step 4: Trying to push to the remote repository..."
    git push

    # If the push fails with SSL certificate problem, try other solutions
    if [ $? -ne 0 ]; then
        echo "Push failed due to SSL certificate problem."
        
        # Option to disable SSL verification (not recommended)
        read -p "Do you want to disable SSL verification (y/n)? " answer
        if [[ "$answer" =~ ^[Yy]$ ]]; then
            disable_ssl_verification
        fi

        # Option to use SSH for remote operations
        read -p "Do you want to use SSH for remote operations (y/n)? " answer
        if [[ "$answer" =~ ^[Yy]$ ]]; then
            use_ssh_for_remote
        fi

        # Retry the Git push after making changes
        echo "Retrying Git push..."
        git push
    else
        echo "Git push successful!"
    fi

    echo "Script execution complete."
}

# Execute the main function
main

