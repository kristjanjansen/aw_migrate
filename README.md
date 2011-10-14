Module for migrating Automatweb CMS data to Drupal

## Installation:

1. Make sure you have Drush installed http://drupal.org/project/drush
2. Create empty database for Drupal
3. Create another database for Automatweb, and import existing data there
4. Download and install Drupal 7 either manually or using Drush
5. Add following snippet to sites/default/settings.php:

```  
$databases['aw']['default'] = array (
  'driver' => 'mysql',
  'database' => 'your_automatweb_database_name',
  'username' => 'your_automatweb_database_username',
  'password' => 'your_automatweb_database_password',
);
```

...and run following commands inside your Drupal site root directory:

```
git clone https://github.com/kristjanjansen/aw_migrate.git sites/all/modules/aw_migrate
drush en aw_migrate -y
drush vset aw_domain http://your_automatweb_site_url_without_trailing_slash
drush mi --all
```

For big sites migrating everything in one go could be very time-consuming, it might
be a good idea to run separate  and/or partial migrations. Run ```drush mi --help``` to find out more. 




