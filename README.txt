Module for migrating AutomaatWeb CMS data to Drupal

Installation:

1. Make sure you have Drush installed http://drupal.org/project/drush
2. Create empty database for Drupal
3. Create another database for AutomaatWeb, call it 'aw' and import existing data there
4. Download and install Drupal 7. Fastest way to do it is to run following commands:

drush dl drupal --drupal-project-rename=drupal
cd drupal
drush si --db-url=mysql://your_db_username:your_db_password@localhost/your_drupal_db_name

5. Add following snippet to sites/default/settings.php:

$databases['aw']['default'] = array (
  'driver' => 'mysql',
  'database' => 'aw',
  'username' => 'your_db_username',
  'password' => 'your_db_username',
  'host' => 'localhost',
  'port' => '',
);

6. Run following commands inside your Drupal site root directory

git clone git@github.com:kristjanjansen/aw_migrate.git sites/all/modules/aw_migrate
drush en aw_migrate -y
drush mi --all








